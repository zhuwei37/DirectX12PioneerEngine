#include"FolderProcessor.h"
#include"GUIDHelper.h"
#include"FileSystem.h"
#include"EngineManager.h"
#include"yaml-cpp/emitter.h"
boost::filesystem::path FolderProcessor::GetDescriptionFilePath(boost::filesystem::path& path)
{

	boost::filesystem::path curPath = path;
	curPath.replace_extension(".meta");
	return curPath;
}

std::shared_ptr<IResource> FolderProcessor::CreateResource(boost::filesystem::path& path)
{
	boost::filesystem::path curPath = path;
	auto descriptionFilePath = GetDescriptionFilePath(curPath);
	if (FileSystem::CreateFolder(curPath))
	{
		if (FileSystem::Create_File(descriptionFilePath))
		{
			auto new_id = IResource::NewID();
			auto guid = GUIDHelper::ResourceIdToString(new_id);
			YAML::Emitter out;
			out << YAML::BeginMap;
			out << YAML::Key << "resourceid";
			out<<YAML::Value<< guid;
			out << YAML::EndMap;
			auto metaString = out.c_str();;
			FileSystem::WriteFile(descriptionFilePath, metaString);
			auto p = path.string();
			auto newResource = std::make_shared< FolderResource>(p, new_id);
			return newResource;
		}

	}
	return nullptr;
}
std::shared_ptr<IResource> FolderProcessor::LoadResource(boost::filesystem::path& path, ResourceManager* manager)
{
	boost::filesystem::path curpath = path;
	auto descriptionFilePath = curpath.replace_extension(".meta");
	if (!FileSystem::IsExist(descriptionFilePath))
	{
		//YAML::Node folder_yaml;
		if (FileSystem::Create_File(descriptionFilePath))
		{
			auto id = IResource::NewID();
			auto guid = GUIDHelper::ResourceIdToString(id);
			YAML::Emitter meta;
			meta << YAML::BeginMap;
			meta << YAML::Key << "resourceid";
			meta<< YAML::Value<< guid;
			meta << YAML::EndMap;
			auto metaString = meta.c_str();
			FileSystem::WriteFile(descriptionFilePath, metaString);
		}
		else
		{
			return nullptr;
		}
	}

	std::string data;
	data = FileSystem::ReadFile(descriptionFilePath);
	auto meta = YAML::Load(data);
	auto guidStr = meta["resourceid"].as<std::string>();
	auto guid = GUIDHelper::StringToResourceId(guidStr);
	auto pathstring = path.string();
	auto resource = std::make_shared< FolderResource>(pathstring, guid);
	return resource;
}

void FolderProcessor::DeleteResource(IResource* resource)
{
	if (resource != nullptr)
	{
		auto folder = dynamic_cast<FolderResource*>(resource);
		if (folder != nullptr)
		{
			boost::filesystem::path path = folder->GetPath();
			FileSystem::DeleteFolder(path);
			auto descriptionFilePath = GetDescriptionFilePath(path);
			FileSystem::Delete_File(descriptionFilePath);
		}
	}
}