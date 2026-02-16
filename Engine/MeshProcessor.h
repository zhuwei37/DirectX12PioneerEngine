#pragma once
#include"IResourceProcessor.h"
class MeshProcessor:public IResourceProcessor
{
public:
	// Í¨¹ý IResourceProcessor ¼Ì³Ð
	std::shared_ptr<IResource> CreateResource(boost::filesystem::path& path) override;
	std::shared_ptr<IResource> CreateResource()override;
	std::shared_ptr<IResource> LoadResource(boost::filesystem::path& path, ResourceManager*) override;
	void DeleteResource(IResource*) override;
	bool DeleteResource(boost::filesystem::path& path) override;
};