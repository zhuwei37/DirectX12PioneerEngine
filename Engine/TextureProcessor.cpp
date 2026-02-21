#include "TextureProcessor.h"
#include"FileSystem.h"
#include"IResource.h"
#include"GUIDHelper.h"
#include"TextureResource.h"
#include"EngineManager.h"
#include"yaml-cpp/emitter.h"
boost::filesystem::path TextureProcessor::GetDescriptionFilePath(boost::filesystem::path& path)
{
    boost::filesystem::path curPath = path;
    std::string extension = curPath.extension().string();
    curPath.replace_extension(extension + ".meta");
	return curPath;
}
std::shared_ptr<IResource> TextureProcessor::CreateResource()
{
	return std::make_shared<TextureResource>();
}
std::shared_ptr<IResource> TextureProcessor::CreateResource(boost::filesystem::path& path)
{
    return std::shared_ptr<TextureResource>();
}

std::shared_ptr<IResource> TextureProcessor::LoadResource(boost::filesystem::path& path, ResourceManager*)
{
    boost::filesystem::path curPath = path;
    auto descriptionFilePath = GetDescriptionFilePath(curPath);
    if (!FileSystem::IsExist(descriptionFilePath))
    {
		if (FileSystem::Create_File(descriptionFilePath))
		{
			auto id = IResource::NewID();
			auto guid = GUIDHelper::ResourceIdToString(id);

			YAML::Emitter out;
			out << YAML::BeginMap;
			out << YAML::Key << "resourceid";
			out << YAML::Value << guid;
			out << YAML::EndMap;
			auto metaString =out.c_str();
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
	auto name= path.filename().string();
	//¼ÓÔØGPU×ÊÔ´
	auto device= EngineManager::Get()->GetGraphicsDevice();
	auto wf = path.wstring();
	auto gTex = GTexture::CreateTextureFromFile(device, wf);
	auto srv = ShaderResourceView::CreateShaderView(device, gTex.get());
	std::shared_ptr<Texture> tex = std::make_shared<Texture>(gTex, srv);
	auto resource = std::make_shared<TextureResource>(pathstring,name, guid,tex);
	return resource;

}

void TextureProcessor::DeleteResource(IResource* resource)
{
	if (resource != nullptr)
	{
		auto texture = dynamic_cast<TextureResource*>(resource);
		if (texture != nullptr)
		{
			boost::filesystem::path path = texture->GetPath();
			FileSystem::Delete_File(path);
			auto descriptionFilePath = GetDescriptionFilePath(path);
			FileSystem::Delete_File(descriptionFilePath);
		}
	}
}

bool TextureProcessor::DeleteResource(boost::filesystem::path& path)
{
    return false;
}
