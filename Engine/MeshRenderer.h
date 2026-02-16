#pragma once
#include"Renderer.h"
#include"Material.h"
#include"interface.h"
#include"Transform.h"
#include"MeshFilter.h"
#include"MaterialResource.h"
#include"Actor.h"
class MeshRenderer:public Renderer
{
public:
	struct MeshRendererItem
	{

	};
	MeshRenderer()=default;
	void Awake()override;
	void Destory()override;
	int GetSubMeshCount();
	DirectX::XMFLOAT4X4 GetTransformMatrix()override;

	void AddMaterial();
	void RemoveMaterial();


	void SetMaterial(int SubMeshIndex,MaterialResource* mat);

	MaterialResource* GetMaterial(int subMeshIndex);

	int GetMaterialCount();

	void SetMeshFilter();
	//MeshFilter* GetMesh()const;
	 void Update()override;
	 const char* GetName()const override;
	 void PreRender()override;
	 void OnRender(ComPtr<ID3D12GraphicsCommandList> commandList , RenderHost* renderHost)override;
	 Mesh* GetMesh()
	 {
		 auto meshFilter = this->m_pOwner->GetComponent<MeshFilter>("MeshFilter");
		 if (meshFilter.expired())
		 {
			 return nullptr;
		 }
		 auto mesh = meshFilter.lock()->GetMesh();
		 return mesh;
	 }
	~MeshRenderer();
private:
	weak_ptr<MeshFilter> mMeshFilter;
	vector<MaterialResource*> mMaterials;
	Transform* transform;
};