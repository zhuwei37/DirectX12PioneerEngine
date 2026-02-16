#pragma once
#include"ExPort.h"
#include"interface.h"
#include"DirectXMath.h"
class IComponent;
extern"C"
{

	DLL_EXPORTS int GetComponentCount(ActorId id);

	DLL_EXPORTS bool GetActorAllComponent(ActorId id, ComponentExportInfo* exportComponentData,int capacity);
	/// <summary>
	/// 添加组件
	/// </summary>
	/// <param name="id"></param>
	/// <param name="componentId"></param>
	/// <param name="componentInfo"></param>
	/// <returns></returns>
	DLL_EXPORTS bool AddComponent(ActorId id, ComponentId componentId,__out ComponentExportInfo& componentInfo);
	/// <summary>
	/// 删除组件
	/// </summary>
	/// <param name="id"></param>
	/// <param name="componentId"></param>
	/// <returns></returns>
	DLL_EXPORTS void RemoveComponent(ActorId id, ComponentId componentId);
	DLL_EXPORTS bool ActorHasComponent(ActorId id, const char* componentName);





#pragma region TransformComponent
	DLL_EXPORTS DirectX::XMFLOAT3 Get_Transform_Position(void* transformPtr);
	DLL_EXPORTS DirectX::XMFLOAT3 Get_Transform_Rotate(IComponent* transformPtr);
	DLL_EXPORTS DirectX::XMFLOAT3 Get_Transform_Scale(IComponent* transformPtr);

	DLL_EXPORTS void Set_Transform_Position(IComponent* transformPtr, DirectX::XMFLOAT3 pos);
	DLL_EXPORTS void Set_Transform_Rotate(IComponent* transformPtr, DirectX::XMFLOAT3 rotate);
	DLL_EXPORTS void Set_Transform_Scale(IComponent* transformPtr, DirectX::XMFLOAT3 scale);
#pragma endregion


#pragma region MeshFilter  网格过滤器
	DLL_EXPORTS bool GetMeshFilter_MeshResourceID(IComponent* meshFilterComponent, ResourceID & resourceId);
	DLL_EXPORTS void SetMeshFilter_MeshResourceID(IComponent* meshFilterComponent, ResourceID resourceId);
#pragma endregion
#pragma region MeshRenderer 网格渲染器
	DLL_EXPORTS int GetMeshRenderer_MaterialCount(IComponent* meshRendererComponent);
	DLL_EXPORTS bool GetMeshRenderer_MaterialResourceIDs(IComponent* meshRendererComponent,__in ResourceID* materialResourceIDs, __inout int& size);
	DLL_EXPORTS bool SetMeshRenderer_MaterialResourceID(IComponent* meshRendererComponent, int index,ResourceID materialResourceID);

	DLL_EXPORTS void MeshRenderer_AddMaterialSlot(IComponent*);
	DLL_EXPORTS void MeshRenderer_RemoveMaterialSlot(IComponent*);
#pragma endregion


}
