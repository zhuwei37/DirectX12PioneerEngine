#include "MaterialProcessor.h"
#include"FileSystem.h"
#include"GUIDHelper.h"
#include"EngineManager.h"
#include"nlohmann/json.hpp"
#include"MaterialResource.h"
#include"MaterialContexts.h"
std::shared_ptr<IResource> MaterialProcessor::CreateResource(boost::filesystem::path& path)
{
	/*nlohmann::json data;
	nlohmann::json proerty;
	nlohmann::json textures;
	proerty[Float4Str]["gDiffuseAlbedo"] = "[1,2,3,4]";
	proerty[Float3Str]["gFresnelR0"] = "sssssss";
	proerty[FloatStr]["gRoughness"] = "1.0";
	data[PropertyStr] = proerty;
	data[TextureStr] = textures;
	auto  jsonDataStr=data.dump();
	FileSystem::Create_File(path);
	FileSystem::WriteFile(path, jsonDataStr);*/
	std::shared_ptr<MaterialResource> mr = std::make_shared<MaterialResource>();
    return mr;
}
std::shared_ptr<IResource> MaterialProcessor::CreateResource()
{
	std::shared_ptr<MaterialResource> mr = std::make_shared<MaterialResource>();
	return mr;
}


std::shared_ptr<IResource> MaterialProcessor::LoadResource(boost::filesystem::path& path, ResourceManager*)
{
	boost::filesystem::path curPath = path;
	auto descriptionFilePath = GetDescriptionFilePath(curPath);
	if (!FileSystem::IsExist(descriptionFilePath))
	{
		if (FileSystem::Create_File(descriptionFilePath))
		{
			auto id = IResource::NewID();
			auto guid = GUIDHelper::ResourceIdToString(id);
			nlohmann::json meta;
			meta["resourceid"] = guid;
			auto metaString = meta.dump();
			FileSystem::WriteFile(descriptionFilePath, metaString);
		}
		else
		{
			return nullptr;
		}
	}
	std::string data;
	data = FileSystem::ReadFile(descriptionFilePath);
	auto meta = nlohmann::json::parse(data);
	auto guidStr = meta["resourceid"];
	auto guid = GUIDHelper::StringToResourceId(guidStr);
	auto pathstring = path.string();
	auto name = path.filename().string();
	//¼ÓÔØGPU×ÊÔ´
	auto device = EngineManager::Get()->GetGraphicsDevice();
	auto wf = path.wstring();
	auto gTex = GTexture::CreateTextureFromFile(device, wf);
	
	return nullptr;
}

void MaterialProcessor::DeleteResource(IResource*)
{
}

bool MaterialProcessor::DeleteResource(boost::filesystem::path& path)
{
    return false;
}
