#include "MeshProcessor.h"
#include"MeshResource.h"
std::shared_ptr<IResource> MeshProcessor::CreateResource(boost::filesystem::path& path)
{
    return std::shared_ptr<IResource>();
}
std::shared_ptr<IResource> MeshProcessor::CreateResource()
{
    std::shared_ptr<MeshResource> resource = std::make_shared<MeshResource>(IResource::NewID());
    return resource;
}
std::shared_ptr<IResource> MeshProcessor::LoadResource(boost::filesystem::path& path, ResourceManager*)
{
    return std::shared_ptr<IResource>();
}

void MeshProcessor::DeleteResource(IResource*)
{
}

bool MeshProcessor::DeleteResource(boost::filesystem::path& path)
{
    return false;
}
