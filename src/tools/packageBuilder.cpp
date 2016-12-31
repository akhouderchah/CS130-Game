#define PACKAGE_MODIFY
#include "Package.h"
#include <iostream>

int main(int argc, char **argv)
{
	Package package;

	if(!package.CreatePackage("test.cpk"))
	   std::cout << "Fuu\n";

	char hd[] = "start";
	package.AddElement("../../assets/textures/Background.tga", hd, sizeof(hd));

	package.Unload();

	return 0;
}
