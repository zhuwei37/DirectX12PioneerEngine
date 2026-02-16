#pragma once
#include"MaterialResource.h"
#include"IResourceProcessor.h"

class MaterialProcessor :public IResourceProcessor
{
public:

	MaterialProcessor() = default;

	/// <summary>
	/// 创建默认资源
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<IResource> CreateResource()override;
	std::shared_ptr<IResource> CreateResource(boost::filesystem::path& path) override;
	std::shared_ptr<IResource> LoadResource(boost::filesystem::path& path, ResourceManager*) override;
	void DeleteResource(IResource*) override;
	bool DeleteResource(boost::filesystem::path& path) override;
	boost::filesystem::path GetDescriptionFilePath(boost::filesystem::path& path)
	{
		boost::filesystem::path curPath = path;
		std::string extension = curPath.extension().string();
		curPath.replace_extension(extension + ".meta");
		return curPath;
	}

};