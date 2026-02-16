#pragma once
#include"FolderResource.h"
#include"IResourceProcessor.h"
class FolderProcessor:public IResourceProcessor
{
public:
	std::shared_ptr<IResource> CreateResource(boost::filesystem::path& path)override;
	std::shared_ptr<IResource> LoadResource(boost::filesystem::path &path, ResourceManager*)override;
	bool DeleteResource(boost::filesystem::path& path)override { return true; }
	void DeleteResource(IResource* resource);
	boost::filesystem::path GetDescriptionFilePath(boost::filesystem::path& path);

private:
};