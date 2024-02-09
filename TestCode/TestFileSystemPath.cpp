#include "pch.h"
#include "TestFileSystemPath.h"

void TestFileSystemPath()
{
	std::filesystem::path path = "D:\\Git\\D3D11_Engine\\Resource";
	std::filesystem::path curr = std::filesystem::current_path();
	std::filesystem::path relative = std::filesystem::relative(path, curr);
	std::cout << relative << std::endl;
}
