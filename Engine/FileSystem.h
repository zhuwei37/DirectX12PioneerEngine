#pragma once
#include"boost/filesystem.hpp"
class FileSystem
{
public:
	static bool Create_File(boost::filesystem::path& path);
	static bool Delete_File(boost::filesystem::path& path);
	static std::string ReadFile(boost::filesystem::path& path);
	static void WriteFile(boost::filesystem::path& path, std::string& str);
	static void WriteFile(boost::filesystem::path& path, const char* str);
	static bool CreateFolder(boost::filesystem::path& path);
	static bool DeleteFolder(boost::filesystem::path& path);
	static bool IsFolder(boost::filesystem::path& path);
	static bool IsFile(boost::filesystem::path& path);
	static bool IsExist(boost::filesystem::path& path);
};