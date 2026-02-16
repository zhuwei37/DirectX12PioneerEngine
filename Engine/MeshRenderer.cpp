#include"MeshRenderer.h"
#include"Actor.h"
#include"RenderHost.h"
#include"MaterialContext.h"
#include"GMaterial.h"
#include"DefaultConstants.h"
#include"EngineManager.h"
#include"RenderSystem.h"
void MeshRenderer::Awake()
{
	transform=this->m_pOwner->GetComponent<Transform>("Transform").lock().get();
	EngineManager::Get()->GetRenderSystem()->ActivateRenderer(this);
}

DirectX::XMFLOAT4X4 MeshRenderer::GetTransformMatrix()
{
	if (transform != nullptr)
	{
		return transform->GetWorldMatrix();
	}
	return {};
}
int MeshRenderer::GetSubMeshCount()
{
	
	if (!mMeshFilter.expired())
	{
		 auto meshfilter=  mMeshFilter.lock();
		 return meshfilter->GetMesh()->SubMeshes.size();
	}
	return 0;
}


void MeshRenderer::SetMaterial(int SubMeshIndex,MaterialResource* mat)
{
	if (SubMeshIndex<this->mMaterials.size())
	{
		this->mMaterials[SubMeshIndex] = mat;
	}
}
MaterialResource* MeshRenderer::GetMaterial(int subMeshIndex)
{
	if (subMeshIndex < mMaterials.size())
	{
		return mMaterials[subMeshIndex];
	}
	return nullptr;
}
int MeshRenderer::GetMaterialCount()
{
	return mMaterials.size();
}
void MeshRenderer::SetMeshFilter()
{
	 auto meshfilter= this->m_pOwner->GetComponent<MeshFilter>("MeshFilter");
	 if (!meshfilter.expired())
	 {
		 auto meshfilterPtr = meshfilter.lock();
		 this->mMeshFilter = meshfilter;
		 this->mMaterials.resize(meshfilterPtr->GetMesh()->SubMeshes.size());
	 }
}
void MeshRenderer::AddMaterial()
{
	mMaterials.push_back(nullptr);
}
void MeshRenderer::RemoveMaterial()
{
	mMaterials.pop_back();
}
void MeshRenderer::PreRender()
{
	
}
void MeshRenderer::Destory()
{

}
void MeshRenderer::OnRender(ComPtr<ID3D12GraphicsCommandList> commandList, RenderHost* renderHost)
{
	/*if (mMeshFilter.expired())
	{
		return;
	}*/
	auto meshFilter=this->m_pOwner->GetComponent<MeshFilter>("MeshFilter");
	if (meshFilter.expired())
	{
		return;
	}
	auto mesh = meshFilter.lock()->GetMesh();
	//auto mesh = mMeshFilter.lock()->GetMesh();
	if (mesh == nullptr||mesh->SubMeshes.size ()< 0)
	{
		return;
	}
	if (mMaterials.size() == 0)
	{
		return;
	}
	//设置顶点 顶点索引
	auto vbv = mesh->VertexBufferView();
	commandList->IASetVertexBuffers(0, 1,&vbv);
	auto ibv = mesh->IndexBufferView();
	commandList->IASetIndexBuffer(&ibv);
	commandList->IASetPrimitiveTopology(mesh->PrimitiveTopology);
	for (int i=0;i<mesh->SubMeshes.size();i++)
	{
		auto subMesh = mesh->SubMeshes[i];
		//启用默认材质
		if (mMaterials.size() <= i)
		{
			return;
		}
		auto matWeakPtr = mMaterials[i];
		
		if (matWeakPtr==nullptr)
		{
			continue;
		}
		{
			auto mat = matWeakPtr->GetMaterial();;
			if (mat == nullptr)
			{
				continue;
			}
			auto gMat = mat->GetGMat();
			if (gMat != nullptr)
			{
				
				auto rootSignature = gMat->mRenderShader->mRootSignature;
				if (rootSignature == nullptr)
				{
					continue;
				}
				commandList->SetGraphicsRootSignature(rootSignature->GetRootSig());
				//固定参数
				//裁剪矩阵、环境光
				commandList->SetGraphicsRootConstantBufferView(0, renderHost->mCommonPassConstantsRenderData.NavigateResource()->GetGPUVirtualAddress());
				//光源
				commandList->SetGraphicsRootConstantBufferView(1, renderHost->mCommonLightRenderData.NavigateResource()->GetGPUVirtualAddress());
				
				//位置矩阵
				//UINT CalcConstantBufferByteSize(UINT byteSize)
//{
//    return (byteSize + 255) & ~255;
//}
				auto size= (sizeof(ObjectPassConstants) + 255) & ~255;
				commandList->SetGraphicsRootConstantBufferView(2
					, renderHost->mCommonObjectPassContantsRenderData.NavigateResource()
					->GetGPUVirtualAddress()+ mObjectPassConstantIndex*size);
				//自定义着色器参数
				gMat->SetGraphicsRoot(commandList.Get());

				for (auto& pass : gMat->mRenderShader->mShaderPasses)
				{
					commandList->SetPipelineState(pass.GetPipline()->mDxPipelineState.Get());
					{
						//触发DrawCall命令
						commandList->DrawIndexedInstanced(subMesh.IndexCount,1, subMesh.IndexBaseStart, subMesh.VertexBaseStart, 0);
					}
				}
			}
		}
	}
	
}
const char* MeshRenderer::GetName()const
{
	const char* name = "MeshRenderer";
	return name;
}
void MeshRenderer::Update()
{
}
MeshRenderer::~MeshRenderer()
{
	EngineManager::Get()->GetRenderSystem()->CloseRenderer(this);
}