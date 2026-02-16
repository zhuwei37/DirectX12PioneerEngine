#include"MeshResource.h"
MeshResource::MeshResource(ResourceID id)
{
	this->mId = id;
	this->mResourceType = ResourceType::MESH;
	mesh = std::make_shared<Mesh>();
}
MeshResource::MeshResource(ResourceID id, GeometryGenerator::MeshData& data, GraphicsDevice* gDevice)
{
	this->mId = id;
	this->mResourceType = ResourceType::MESH;
	mesh = std::make_shared<Mesh>();
	mesh->init(data, gDevice);
}