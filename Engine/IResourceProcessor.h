#pragma once
#include"boost/filesystem.hpp"
class IResource;
class ResourceManager;
class IResourceProcessor
{
public:
	virtual std::shared_ptr<IResource> CreateResource(boost::filesystem::path& path)=0;
	virtual std::shared_ptr<IResource> CreateResource() { return nullptr; }
	virtual std::shared_ptr<IResource> LoadResource(boost::filesystem::path& path,ResourceManager*)=0;
	virtual std::shared_ptr<IResource>  ImportResource(boost::filesystem::path& path) { return std::make_shared<IResource>(); }
	virtual void DeleteResource(IResource*) = 0;
	virtual bool DeleteResource(boost::filesystem::path& path) = 0;
	virtual void SaveResource(std::shared_ptr<IResource> resource) {}
};