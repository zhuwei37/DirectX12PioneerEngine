#pragma once
#include"IResource.h"
#include"Mesh.h"
class MeshResource :public IResource
{
	friend class ResourceManager;
	friend class ResourceSystem;
public:
	MeshResource(ResourceID id);
	MeshResource(ResourceID, GeometryGenerator::MeshData& data,GraphicsDevice* gDevice);
	Mesh* GetMesh() 
	{
		return mesh.get();
	}
private:
	std::shared_ptr<Mesh> mesh;
};