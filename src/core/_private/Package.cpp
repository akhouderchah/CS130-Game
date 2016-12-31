#include "Package.h"
#include "Platform.h"
#include "ErrorSystem.h"
#include <cstddef>
#include <cstdio>

Package::Package()
#ifdef PACKAGE_MODIFY
	: m_IsModified(false)
#endif
{
}

Package::~Package()
{
	if(!Unload())
	{
		LOG("Failed to close package properly!\n");
	}

#ifdef PACKAGE_MODIFY
	// Unload() should close the fd, but in the case that there is an error
	// flushing the data, we'll still want to close everything (the user can't
	// handle the error at this point; if not losing the data is a concern,
	// either FlushChanges() or Unload() should've been called manually before
	// this point).
	if(m_PackageFile.is_open())
	{
		m_PackageFile.close();
	}
#endif
}

// @TODO log all errors
bool Package::Load(const std::string &path)
{
	PackageFormat::TableElement tableElem;

	// Can't already have an open package
	if(m_PackageFile.is_open()){ return false; }

	// Open the package file
	m_PackageFile.open(path, std::ios::in | std::ios::out | std::ios::binary);
	if(!m_PackageFile.is_open()){ return false; }

	// Load the header into memory
	m_PackageFile.read((char*)&m_Header, sizeof(m_Header));
	if(!m_PackageFile){ goto exit; }

	// @TODO Ensure header validity
	if(m_Header.ID[0] != PackageFormat::ID[0] ||
	   m_Header.ID[1] != PackageFormat::ID[1] ||
	   m_Header.ID[2] != PackageFormat::ID[2]){ goto exit; }

	for(size_t i = 0; i < sizeof(PackageFormat::projID_t); ++i)
	{
		if(m_Header.ProjID[i] != PackageFormat::PROJ_ID[i]){ goto exit; }
	}

	#ifdef IS_BIG_ENDIAN
	// Flip if needed
	bxchg16(m_Header.FormatVersion);
	bxchg16(m_Header.MinReaderVersion);
	bxchg16(m_Header.ItemCount);
	bxchg32(m_Header.TablePosition);
	#endif

	if(m_Header.MinReaderVersion > PackageFormat::MIN_READER_VERSION){ goto exit; }

	// Load the table into memory
	m_PackageFile.seekg(m_Header.TablePosition);

	for(size_t i = 0; i < m_Header.ItemCount; ++i)
	{
		if(!m_PackageFile.read((char*)&tableElem, sizeof(tableElem)))
		{
			goto exit;
		}

		// Flip if needed
		#ifdef IS_BIG_ENDIAN
		bxchg32(tableElem.first);
		bxchg32(tableElem.second);
		#endif
		m_HashToPos.insert(tableElem);
	}

#ifdef PACKAGE_MODIFY
	{
		// Store filename
		m_PackagePath = path;

		// Load in all data
		size_t dataStart = m_PackageFile.tellg();
		m_PackageFile.seekg(0, std::ios::end);
		size_t dataEnd = m_PackageFile.tellg();

		m_FileContents.resize(dataEnd - dataStart);
		m_PackageFile.seekg(dataStart);
		m_PackageFile.read((char*)&m_FileContents[0], dataEnd - dataStart);
	}
#endif

	return true;

exit:
	m_PackageFile.close();
	return false;
}

bool Package::Unload()
{
#ifdef PACKAGE_MODIFY
	if(!FlushChanges())
	{
		return false;
	}

	m_FileContents.clear();
#endif

	if(m_PackageFile.is_open())
	{
		m_PackageFile.close();
	}

	m_HashToPos.clear();

	return true;
}

// @TODO log all errors
void *Package::AllocExtract(const std::string& filename, void *pSubHeader,
							size_t subHeaderSize, size_t *pSize)
{
	// Set file position to data element
	auto iter = m_HashToPos.find(Hash(filename));
	if(iter == m_HashToPos.end())
	{
		return nullptr;
	}
	m_PackageFile.seekg(iter->second);

	PackageFormat::DataHeader header;
	m_PackageFile.read((char*)&header, sizeof(header));
	if(!m_PackageFile){ return nullptr; }

	// Flip if needed
	#ifdef IS_BIG_ENDIAN
	bxchg16(header.HeaderSize);
	bxchg32(header.DataSize);
	#endif

	// Read in sub-header
	if(pSubHeader)
	{
		if(header.HeaderSize > subHeaderSize)
		{
			LOG("Subheader buffer smaller than subheader data!\n");
			return nullptr;
		}

		m_PackageFile.read((char*)pSubHeader, header.HeaderSize);
		if(!m_PackageFile){ return nullptr; }
	}
	else
	{
		m_PackageFile.seekg(header.HeaderSize, m_PackageFile.cur);
	}

	// Read in data
	void *pData = ::operator new(header.DataSize);
	m_PackageFile.read((char*)pData, header.DataSize);
	if(!m_PackageFile){ UnallocData(pData); return nullptr; }

	// Return relevant info
	if(pSize)
	{
		*pSize = header.DataSize;
	}
	return pData;
}

void Package::UnallocData(void *pData)
{
	::operator delete(pData);
}

#ifdef PACKAGE_MODIFY
bool Package::CreatePackage(const std::string &path)
{
	if(m_PackageFile.is_open())
	{
		LOG("Cannot create new package when existing package is open.\n");
		return false;
	}

	m_PackageFile.open(path, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
	if(!m_PackageFile.is_open())
	{
		LOG("Failed to create package: " << path << ".\n");
		return false;
	}

	m_Header.ID[0] = PackageFormat::ID[0];
	m_Header.ID[1] = PackageFormat::ID[1];
	m_Header.ID[2] = PackageFormat::ID[2];

	for(size_t i = 0; i < sizeof(PackageFormat::projID_t); ++i)
	{
		m_Header.ProjID[i] = PackageFormat::PROJ_ID[i];
	}

	m_Header.EncryptionType = PackageFormat::EncryptionType_t::NO_ENCRYPTION;
	m_Header.Reserved[0] = 0; m_Header.Reserved[1] = 0; m_Header.Reserved[2] = 0;
	m_Header.FormatVersion = PackageFormat::CURRENT_VERSION;
	m_Header.MinReaderVersion = PackageFormat::MIN_READER_VERSION;
	m_Header.ItemCount = 0;
	m_Header.CompressionType = PackageFormat::CompressionType_t::NO_COMPRESSION;
	m_Header.ChecksumType = PackageFormat::ChecksumType_t::NO_CHECKSUM;
	m_Header.TablePosition = sizeof(m_Header);

	m_IsModified = true;
	m_PackagePath = path;

	return true;
}

bool Package::AddElement(const std::string &filename, void *pSubHeader, size_t subHeaderSize)
{
	DEBUG_ASSERT(pSubHeader);

	//// Hash filename and ensure unique
	uint32_t hash = Hash(filename);
	auto iter = m_HashToPos.find(hash);
	if(iter != m_HashToPos.end())
	{
		LOG("Non-unique hash value! Cannot add file to package.\n");
		return false;
	}

	if(m_Header.ItemCount == UINT16_MAX)
	{
		LOG("Cannot add any more files to this package.\n");
		return false;
	}

	//// Open file
	std::ifstream dataFile(filename, std::ios::in | std::ios::binary);
	if(!dataFile)
	{
		LOG("Failed to open file: " << filename << ".\n");
		return false;
	}

	// Any errors from here on must first undo whatever changes the code has made
	++m_Header.ItemCount;

	//// Add data
	PackageFormat::DataHeader dataHeader;
	dataHeader.HeaderSize = subHeaderSize;
	dataHeader.Reserved[0] = 0; dataHeader.Reserved[1] = 0;

	// Set data size to data file size
	dataFile.seekg(0, std::ios::end);
	size_t dataSize = dataFile.tellg();
	dataHeader.DataSize = dataSize;

#ifdef IS_BIG_ENDIAN
	// Flip if needed
	bxchg16(dataHeader.HeaderSize);
	bxchg32(dataHeader.DataSize);
#endif

	size_t elemStart = m_FileContents.size();

	// Store data header in memory
	for(size_t i = 0; i < sizeof(dataHeader); ++i)
	{
		m_FileContents.push_back(((uint8_t*)&dataHeader)[i]);
	}

	// Store sub-header in memory
	for(size_t i = 0; i < subHeaderSize; ++i)
	{
		m_FileContents.push_back(((uint8_t*)pSubHeader)[i]);
	}

	// Read file data into memory
	size_t dataStart = m_FileContents.size();
	m_FileContents.resize(dataStart + dataSize);
	dataFile.seekg(0);
	dataFile.read((char*)&m_FileContents[dataStart], dataSize);
	dataFile.close();

	// Increase all values in hash table by sizeof(TableElement)
	for(auto iter = m_HashToPos.begin(); iter != m_HashToPos.end(); ++iter)
	{
		iter->second += sizeof(PackageFormat::TableElement);
	}

	// Add new value into hash table
	m_HashToPos[hash] = elemStart + m_Header.TablePosition + m_Header.ItemCount*sizeof(PackageFormat::TableElement);

	m_IsModified = true;
	return false;
}

bool Package::RemoveElement(const std::string &filename)
{
	// @TODO

	// Hash filename
	uint32_t hash = Hash(filename);
	auto iter = m_HashToPos.find(hash);
	if(iter == m_HashToPos.end())
	{
		LOG("Hash value does not exist in file table! Cannot delete anything.\n");
		return false;
	}

	// Modify header and table accordingly
	--m_Header.ItemCount;

	// Remove data

	m_IsModified = true;
	return false;
}

bool Package::FlushChanges()
{
	// Write changes to disk if needed
	if(m_IsModified)
	{
		DEBUG_ASSERT(m_PackageFile.is_open());
		//// Write to tmp file first
		std::ofstream tmpFile(m_PackagePath + ".tmp", std::ios::trunc | std::ios::binary);
		if(!tmpFile)
		{
			LOG("Failed to open the package file: " << m_PackagePath + ".tmp" << ".\n");
			return false;
		}

		// Write header
#ifdef IS_BIG_ENDIAN
		// Flip if needed
		PackageFormat::Header tmpHeader = m_Header;
		bxchg16(m_Header.FormatVersion);
		bxchg16(m_Header.MinReaderVersion);
		bxchg16(m_Header.ItemCount);
		bxchg32(m_Header.TablePosition);
#endif
		if(!tmpFile.write((char*)&m_Header, sizeof(m_Header))){ goto error; }

		// Write table
		for(auto iter = m_HashToPos.begin(); iter != m_HashToPos.end(); ++iter)
		{
			PackageFormat::TableElement elem = *iter;

#ifdef IS_BIG_ENDIAN
			bxchg32(elem.first);
			bxchg32(elem.second);
#endif
			if(!tmpFile.write((char*)&elem.first, sizeof(elem.first))){ goto error; }
			if(!tmpFile.write((char*)&elem.second, sizeof(elem.second))){ goto error; }
		}

#ifdef IS_BIG_ENDIAN
		// Restore header
		m_Header = tmpHeader;
#endif

		// Write data
		if(m_FileContents.size() > 0)
		{
			if(!tmpFile.write((char*)&m_FileContents[0], m_FileContents.size())){ goto error; }
		}

		tmpFile.close();

		//// Replace original file
		m_PackageFile.close();
		if(remove(m_PackagePath.c_str()))
		{
			LOG("Failed to remove original package file.\n");
			return false;
		}
		if(rename((m_PackagePath + ".tmp").c_str(), m_PackagePath.c_str()))
		{
			LOG("ERROR: Failed to rename temp package file to actual package file.\n");
			return false;
		}
		m_PackageFile.open(m_PackagePath);
		if(!m_PackageFile.is_open())
		{
			LOG("Failed to re-open package file.\n");
			return false;
		}

		m_IsModified = false;
		return true;

	error:
		LOG("Failed to write to the temp package file.\n");
		tmpFile.close();
		return false;
	}

	return true;
}
#endif // PACKAGE_MODIFY

/**
 * This hash function was adapted from the following link and was not
 * written by me:
 ** http://stackoverflow.com/questions/8317508/hash-function-for-a-string
*/
uint32_t Package::Hash(const std::string &str)
{
	const uint32_t A = 54059; /* a prime */
	const uint32_t B = 76963; /* another prime */
	// const uint32_t C = 86969; /* yet another prime */
	const uint32_t FIRSTH = 37; /* also prime */
	const char *s = str.c_str();

	uint32_t h = FIRSTH;
	while (*s)
	{
		h = (h * A) ^ (s[0] * B);
		s++;
	}
	return h; // or return h % C;
}
