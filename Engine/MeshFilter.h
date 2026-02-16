#pragma once
#include"IComponent.h"
#include"Mesh.h"
#include"MeshResource.h"
class MeshFilter:public IComponent
{
public:
	MeshFilter();
	// Í¨¹ý IComponent ¼Ì³Ð
	void Awake() override;
	void Update() override;
	const char* GetName() const override;

	MeshResource* GetMeshResource();
	void SetMeshResource(std::shared_ptr<MeshResource> mesh);

	Mesh* GetMesh()
	{
		if (mMeshResource != nullptr)
		{
			return mMeshResource->GetMesh();
		}
		return nullptr;
	}



private:
	std::shared_ptr<MeshResource> mMeshResource;
};