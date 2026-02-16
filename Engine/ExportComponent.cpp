#include"ExportComponent.h"
#include"Transform.h"
#include"MeshRenderer.h"
#include"MaterialResource.h"
#include"EngineManager.h"
#include "interface_resource.h"
int GetComponentCount(ActorId actorId)
{
	auto engine = EngineManager::Get();
	auto actor = engine->GetActorManager()->GetActor(actorId);
	if (actor != nullptr)
	{
		return actor->GetComponentCount();
	}
	return 0;
}
bool GetActorAllComponent(ActorId actorId, ComponentExportInfo* exportComponentData, int capacity)
{
	auto engine = EngineManager::Get();
	auto actor = engine->GetActorManager()->GetActor(actorId);
	if (actor != nullptr)
	{
		auto componentCount= actor->GetComponentCount();
		if (componentCount == capacity)
		{
			auto componentInfos = actor->GetComponentInfos();
			/*for (int i = 0; i < capacity; i++)
			{
				exportComponentData[i] = componentInfos[i];
			}*/
			//存在字节序问题
			memcpy(exportComponentData, componentInfos.data(), (sizeof(ComponentExportInfo) * componentCount));
			return true;
		}
	}
	return false;
}


bool AddComponent(ActorId actorId, ComponentId componentid,__out ComponentExportInfo& info)
{
	info.ComponentId = 0;
	info.ComponentPtr = 0;
	auto actorManager = EngineManager::Get()->GetActorManager();
	auto result =actorManager ->AddComponentToActor(actorId, componentid);
	if (result)
	{
		auto actor=actorManager->GetActor(actorId);
		auto componentptr=actor->GetComponentPtr(componentid);
		if (componentptr != nullptr)
		{
			info.ComponentId = componentid;
			info.ComponentPtr = componentptr;
			return true;
		}
	}
	return false;
}

void RemoveComponent(ActorId actorId, ComponentId componentId)
{
	EngineManager::Get()->GetActorManager()->RemoveComponentFromActor(actorId, componentId);
}


#pragma region 变换
DirectX::XMFLOAT3 Get_Transform_Position(void* transformPtr)
{
	
	Transform* transform = dynamic_cast<Transform*>( reinterpret_cast<IComponent*>( transformPtr));
	if (transform != nullptr)
	{
		return transform->GetPosition();
	}
	return {};
}
DirectX::XMFLOAT3 Get_Transform_Rotate(IComponent* transformPtr)
{
	Transform* transform = dynamic_cast<Transform*>(transformPtr);
	if (transform != nullptr)
	{
		return transform->GetRotate();
	}
	return {};
}
DirectX::XMFLOAT3 Get_Transform_Scale(IComponent* transformPtr)
{
	Transform* transform = dynamic_cast<Transform*>(transformPtr);
	if (transform != nullptr)
	{
		return transform->GetScale();
	}
	return {};
}

void Set_Transform_Position(IComponent* transformPtr, DirectX::XMFLOAT3 pos)
{
	Transform* transform = dynamic_cast<Transform*>(transformPtr);
	if (transform != nullptr)
	{
		 transform->SetPosition(pos.x, pos.y, pos.z);
	}
}
void Set_Transform_Rotate(IComponent* transformPtr, DirectX::XMFLOAT3 rotate)
{
	Transform* transform = dynamic_cast<Transform*>(transformPtr);
	if (transform != nullptr)
	{
		 transform->SetRotate(rotate.x, rotate.y, rotate.z);
	}
}
void Set_Transform_Scale(IComponent* transformPtr, DirectX::XMFLOAT3 scale)
{
	Transform* transform = dynamic_cast<Transform*>(transformPtr);
	if (transform != nullptr)
	{
		 transform->SetScale(scale.x, scale.y, scale.z);
	}
}
#pragma endregion


#pragma region 网格过滤器
bool GetMeshFilter_MeshResourceID(IComponent* componentPtr, ResourceID& resourceId) 
{
	if (componentPtr != nullptr)
	{
		auto meshFilter = dynamic_cast<MeshFilter*>(componentPtr);
		if (meshFilter != nullptr)
		{
			auto resource=meshFilter->GetMeshResource();
			if (resource != nullptr)
			{
				resourceId= resource->GetId();
				
			}
			else
			{
				resourceId = RESOURCEID_NONE;
			}
			return true;
		}
	}
	return false;
}

void SetMeshFilter_MeshResourceID(IComponent* componentPtr, ResourceID resourceId)
{
	if (componentPtr != nullptr)
	{
		auto meshFilter = dynamic_cast<MeshFilter*>(componentPtr);
		if (meshFilter != nullptr)
		{
			auto resource= EngineManager::Get()->GetResourceSystem()->GetResourceManager()->GetResourceStrongPtr(resourceId);
			auto meshResource = std::dynamic_pointer_cast<MeshResource>(resource);
			meshFilter->SetMeshResource(meshResource);
		}
	}
}

#pragma endregion

#pragma region 网格渲染器
int GetMeshRenderer_MaterialCount(IComponent* meshRendererComponent)
{
	if (meshRendererComponent != nullptr)
	{
		auto meshRenderer = dynamic_cast<MeshRenderer*>(meshRendererComponent);
		if (meshRenderer != nullptr)
		{
			return meshRenderer->GetMaterialCount();
		}
	}
	return 0;
}

bool GetMeshRenderer_MaterialResourceIDs(IComponent* meshRendererComponent, __in ResourceID* materialResourceIDs, __inout int& size)
{
	if (meshRendererComponent != nullptr)
	{
		auto meshRenderer = dynamic_cast<MeshRenderer*>(meshRendererComponent);
		if (meshRenderer != nullptr)
		{
			if (meshRenderer->GetMaterialCount() == size)
			{
				for (int i = 0; i < size; i++)
				{
					auto materialResource = meshRenderer->GetMaterial(i);
					if (materialResource != nullptr)
					{
						materialResourceIDs[i] = materialResource->GetId();
					}
					else
					{
						materialResourceIDs[i] = RESOURCEID_NONE;
					}
				}
				return true;
			}
		}
	}
	return false;
}
bool SetMeshRenderer_MaterialResourceID(IComponent* meshRendererComponent, int index, ResourceID materialResourceID)
{
	if (meshRendererComponent != nullptr)
	{
		auto meshRenderer = dynamic_cast<MeshRenderer*>(meshRendererComponent);
		if (meshRenderer != nullptr)
		{
			if (materialResourceID == RESOURCEID_NONE)
			{
				meshRenderer->SetMaterial(index, nullptr);
			}
			else
			{
				auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
				auto materialResource=  resourceManager->GetResource(materialResourceID);
				meshRenderer->SetMaterial(index, (MaterialResource*)materialResource);
			}
			return true;
		}
	}
	return false;
}


void MeshRenderer_AddMaterialSlot(IComponent* component)
{
	if (component != nullptr)
	{
		auto meshRenderer = dynamic_cast<MeshRenderer*>(component);
		if (meshRenderer != nullptr)
		{
			meshRenderer->AddMaterial();
		}
	}
}
void MeshRenderer_RemoveMaterialSlot(IComponent* component)
{
	if (component != nullptr)
	{
		auto meshRenderer = dynamic_cast<MeshRenderer*>(component);
		if (meshRenderer != nullptr)
		{
			meshRenderer->RemoveMaterial();
		}
	}
}
#pragma endregion





