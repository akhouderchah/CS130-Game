#define PACKAGE_DEBUG
#include "Package.h"
#include <iostream>

using namespace std;

int main()
{
	Package package;

	string path = "test.cpk";
	package.Load(path);

	// Output header info
	cout << "---- Package info ----\n\n";
	cout << "Package name: " << path << "\n\n";
	cout << "File ID: " << package.m_Header.ID[0] << package.m_Header.ID[1] <<
		package.m_Header.ID[2] << "\n";
	cout << "Project ID: ";
	for(size_t i = 0; i < sizeof(PackageFormat::projID_t); ++i)
	{
		cout << package.m_Header.ProjID[i];
	}
	cout << "\n";
	cout << "Data table starts at: " << package.m_Header.TablePosition << "\n";
	cout << "Item count: " <<  package.m_Header.ItemCount << "\n";

	// Output data element info
	size_t i = 0;
	PackageFormat::DataHeader header;
	for(auto iter = package.m_HashToPos.begin(); iter != package.m_HashToPos.end();
		++iter, ++i)
	{
		cout << "\n\tItem " << i << " hash: " << iter->first << "\n";
		cout << "\tItem " << i << " data starts at: " << iter->second << "\n";

		// Read in item header and data
		package.m_PackageFile.seekg(iter->second);
		package.m_PackageFile.read((char*)&header, sizeof(header));

		// Flip if needed
#ifdef IS_BIG_ENDIAN
		bxchg16(header.HeaderSize);
		bxchg32(header.DataSize);
#endif
		cout << "\tItem " << i << " sub-header size: " << header.HeaderSize << "\n";
		cout << "\tItem " << i << " data size: " << header.DataSize << "\n";

		package.m_PackageFile.seekg(header.HeaderSize, ios::cur);
		string data;
		data.resize(header.DataSize);
		package.m_PackageFile.read((char*)&data[0], header.DataSize);

		ofstream outFile("dataElem" + to_string(i), ios::binary | ios::trunc);
		outFile.write((char*)&data[0], header.DataSize);
		outFile.close();
		cout << "\tItem " << i << " data written to file: " << "dataElem" << to_string(i) << "\n";
	}

	return 0;
}
