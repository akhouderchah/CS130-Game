#pragma once

#include <fstream>
#include <vector>
#include <unordered_map>
#include "PackageFormat.h"

/**
 * @brief Class to handle the use of the asset package format
 *
 * Contains the functionality needed to create and use the asset package format
 * as described in PackageFormat.h. Designed to be both forward and backward
 * compatible.
 *
 * @note Changing the implementation of the hash function necessitates incrementing
 *       the file format version and setting the MinReaderVersion to this new
         version number.
 * @note The functionality for actually modifying the package format (as opposed
 *       to just reading in the data) is only included if PACKAGE_MODIFY is
 *       #defined before including this header. Thus a game itself will not
 *       include the functionality by default, while different tools could. When
 *       PACKAGE_MODIFY is defined, loading a file will read all of its contents
 *       into memory.
 * @note The current implementation simply has all packages stored as little
 *       endian, but a flag could later be added that would allow writing the
 *       file with any endianness, placing the onus of figuring out how to swap
 *       bytes on the newer implementation of Load, Extract, AddElement, and
 *       RemoveElement.
 * @TODO Take care of endianness issues
 */
class Package
{
public:
	Package();
	~Package();

	bool Load(const std::string &path);
	bool Unload();

	/**
	 * @brief Extract asset from package, allocating the data buffer
	 *
	 * @param[in]  filename      - Name of asset
	 * @param[out] pSubHeader    - location of subheader buffer
	 * @param[in]  subHeaderSize - size of subheader buffer
	 * @param[out] pSize         - size of returned data
	 * @return                   - allocated buffer with data
	 */
	void *AllocExtract(const std::string &filename, void *pSubHeader,
					   size_t subHeaderSize, size_t *pSize=nullptr);
	void UnallocData(void *pData);

#define PACKAGE_MODIFY // @TODO Remove after developing!
#ifdef PACKAGE_MODIFY
	bool CreatePackage(const std::string &path); // @TODO pass in more settings
	bool AddElement(const std::string &filename, void *pSubHeader, size_t subHeaderSize); // @TODO ENDIANNESS ISSUES FOR SUBHEADER!
	bool RemoveElement(const std::string &filename);
	bool FlushChanges();
#endif //PACKAGE_MODIFY

private:
	uint32_t Hash(const std::string &str);

private:
#ifdef PACKAGE_DEBUG
public:
#endif
	std::fstream m_PackageFile;
	PackageFormat::Header m_Header;
	std::unordered_map<uint32_t, uint32_t> m_HashToPos;
#ifdef PACKAGE_MODIFY
	bool m_IsModified;
	std::string m_PackagePath;
	std::vector<uint8_t> m_FileContents;
#endif
};
