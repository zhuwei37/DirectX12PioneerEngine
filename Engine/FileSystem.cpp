#include"FileSystem.h"
#include<fstream>
bool FileSystem::Create_File(boost::filesystem::path& path)
{
	bool exist = IsExist(path);
	if (exist)
	{
		bool isFile = IsFile(path);
		if (isFile)
		{
			/*boost::filesystem::rename*/
			return false;
		}
	}
	auto parentFolderPath = path.parent_path();
	exist = IsExist(parentFolderPath);
	bool isFolder = IsFolder(parentFolderPath);
	if (!exist||!isFolder)
	{
		CreateFolder(parentFolderPath);
	}

	std::ofstream ofs(path.string(), std::fstream::out);
	return ofs.is_open();
}
bool FileSystem::Delete_File(boost::filesystem::path& path)
{
	bool exist = IsExist(path);
	if (exist)
	{
		bool isFile = IsFile(path);
		if (isFile)
		{
			return boost::filesystem::remove(path);
		}
	}
	return false;
}
std::string FileSystem::ReadFile(boost::filesystem::path& path)
{
	bool exist = IsExist(path);
	if (exist)
	{
		bool isFile = IsFile(path);
		std::ifstream ifs(path.string());
		std::string data = std::string(std::istreambuf_iterator<char>(ifs),
			std::istreambuf_iterator<char>());
		return data;
	}
	return "";
}
void FileSystem::WriteFile(boost::filesystem::path& path, const char* str)
{
	bool exist = IsExist(path);
	if (exist)
	{
		bool isFile = IsFile(path);
		if (isFile)
		{
			std::ofstream ofs(path.string());
			ofs << str;
		}
	}
}
void FileSystem::WriteFile(boost::filesystem::path& path, std::string& str)
{
	bool exist = IsExist(path);
	if (exist)
	{
		bool isFile = IsFile(path);
		if (isFile)
		{
			std::ofstream ofs(path.string());
			ofs << str;
		}
	}
}
bool FileSystem::CreateFolder(boost::filesystem::path& path)
{
	bool exist = IsExist(path);
	if (exist)
	{
		bool isFolder = IsFolder(path);
		if (isFolder)
		{
			return false;
		}
	}
	else
	{
		return boost::filesystem::create_directories(path);
	}
	return false;
}
bool FileSystem::DeleteFolder(boost::filesystem::path& path)
{
	bool exist = IsExist(path);
	if (exist)
	{
		bool isFolder = IsFolder(path);
		if (isFolder)
		{
			return boost::filesystem::remove(path);
		}
	}
	return true;
}
bool FileSystem::IsFolder(boost::filesystem::path& path)
{
	return boost::filesystem::is_directory(path);
}
bool FileSystem::IsFile(boost::filesystem::path& path)
{
	return boost::filesystem::is_regular_file(path);
}
bool FileSystem::IsExist(boost::filesystem::path& path)
{
	return boost::filesystem::exists(path);
}