#define PACKAGE_DEBUG
#include "Package.h"
#include <iostream>

using namespace std;

int main()
{
	Package package;

	string path = "test.cpk";
	package.Load(path);

	cout << "---- Package info ----\n\n";
	cout << "Package name: " << path << "\n\n";
	cout << "Item count: " <<  package.m_Header.ItemCount << "\n";

	cout << "\n";

	size_t i = 0;
	for(auto iter = package.m_HashToPos.begin(); iter != package.m_HashToPos.end();
		++iter)
	{

	}

	return 0;
}
