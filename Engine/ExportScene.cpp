#include"ExportScene.h"
#include"RenderSystem.h"
#include"EngineManager.h"
#include"GeometryGenerator.h"
#include"MaterialContexts.h"
#include"MaterialResource.h"
#include"TextureResource.h"

int GetSceneNodeCount()
{
	auto scene= EngineManager::Get()->GetMainScene();
	if (scene != nullptr)
	{
		return scene->GetAllSceneNodeCount();
	}
	return 0;
}
bool GetAllSceneNodeInfos(SceneNodeInfo* data, int len)
{
	auto scene = EngineManager::Get()->GetMainScene();
	if (scene != nullptr)
	{
		return scene->GetAllSceneNodeInfos(data, len);
	}
	return false;
}

SceneNodeInfo CreateSceneNode(const char* name, unsigned int parent)
{

	auto scene = EngineManager::Get()->GetMainScene();
	auto actorManager = EngineManager::Get()->GetActorManager();
	if (scene != nullptr)
	{
		auto sceneNodeId = scene->AddSceneNode(name, parent);
		auto actor = actorManager->CreateActor();
		scene->SceneNodeBindActor(sceneNodeId, actor.get());
		actor->SetSceneNode(sceneNodeId);
		SceneNodeInfo info;
		info.ActorId = actor->GetId();
		info.SceneNodeID = sceneNodeId;
		return info;
	}
	return {};
}

void DeleteSceneNode(unsigned int sceneNodeID)
{
	auto scene = EngineManager::Get()->GetMainScene();
	if (scene != nullptr)
	{
		scene->RemoveSceneNode(sceneNodeID);
	}
}

bool GetSceneNodeInfosByID(unsigned int id, SceneNodeInfo* data, int len)
{

	auto scene= EngineManager::Get()->GetMainScene();
	if (scene != nullptr)
	{
		return scene->GetAllSceneNodeCount();
	}
	return 0;
}

void SetSceneNodeName(unsigned int id, char* newName)
{
	auto scene = EngineManager::Get()->GetMainScene();
	  scene->GetSceneNodeByID(id)->SetName(newName);
}


//bool AddComponent(SceneNodeID sceneNodeId, ComponentId compoentId)
//{
//	auto scene = EngineManager::Get()->GetMainScene();
//	auto actorManager = EngineManager::Get()->GetActorManager();
//	if (scene != nullptr)
//	{
//		auto sceneNode = scene->GetSceneNodeByID(sceneNodeId);
//		if (sceneNode != nullptr)
//		{
//			auto actorId = dynamic_cast<SceneNode*>(sceneNode)->GetActorId();
//			actorManager->AddComponentToActor(actorId, compoentId);
//		}
//	}
//	return false;
//}
//bool RemoveComponent(SceneNodeID sceneNodeId, ComponentId compoentId)
//{
//	auto scene = EngineManager::Get()->GetMainScene();
//	auto actorManager = EngineManager::Get()->GetActorManager();
//	if (scene != nullptr)
//	{
//		auto sceneNode = scene->GetSceneNodeByID(sceneNodeId);
//		if (sceneNode != nullptr)
//		{
//			auto actorId = dynamic_cast<SceneNode*>(sceneNode)->GetActorId();
//			actorManager->RemoveComponentFromActor(actorId, compoentId);
//		}
//	}
//	return false;
//}
void LoadScene()
{
	
	CreateSceneNode("Cube", 0);
	//CreateSceneNode("GameObject", 0);
}

ComponentId StringToComponentId(const char* componentName)
{
	return IComponent::GetIdFromName(componentName);
}

void SetAll()
{//ÓÅ»¯
	auto scene = EngineManager::Get()->GetMainScene();
	auto actorManager = EngineManager::Get()->GetActorManager();
	auto renderSystem = EngineManager::Get()->GetRenderSystem();
	auto resourceSystem = EngineManager::Get()->GetResourceSystem();
	auto sceneNode = dynamic_cast<SceneNode*>(scene->GetSceneNodeByID(1));
	auto actor = actorManager->GetActor(sceneNode->GetActorId());
	if (actor != nullptr)
	{

		actorManager->AddComponentToActor(actor->GetId(), "MeshFilter");
		weak_ptr<MeshFilter> meshfilterWeakPtr = actor->GetComponent<MeshFilter>("MeshFilter");
		if (!meshfilterWeakPtr.expired())
		{
			auto meshfilter = meshfilterWeakPtr.lock();

			/*GeometryGenerator gen;
			auto box = gen.CreateBox(1.0f, 1.0f, 1.0f, 3);

			shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
			mesh->mIndices = box.Indices32;
			mesh->mVertices.resize(box.Vertices.size());
			for (int i = 0; i < box.Vertices.size(); i++)
			{
				mesh->mVertices[i].Position = box.Vertices[i].Position;
				mesh->mVertices[i].Normal = box.Vertices[i].Normal;
				mesh->mVertices[i].TexUV1 = box.Vertices[i].TexC;
			}
			mesh->mVertexBufferSize = sizeof(GVertex) * mesh->mVertices.size();
			mesh->mIndexBufferSize = sizeof(UINT) * mesh->mIndices.size();
			mesh->BindingGraphicsResource(EngineManager::Get()->GetGraphicsDevice());
			SubMeshDescriptor sub;
			sub.IndexBaseStart = 0;;
			sub.IndexCount = mesh->mIndices.size();
			sub.VertexBaseStart = 0;
			sub.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			mesh->SubMeshes.push_back(sub);*/

			auto boxid = GUIDHelper::StringToResourceId(BOX_MESH_RESOURCEID_STR);
			auto meshResource = std::dynamic_pointer_cast<MeshResource>(
				resourceSystem->GetResourceManager()->GetResourceStrongPtr(boxid));

			meshfilter->SetMeshResource(meshResource);




			actorManager->AddComponentToActor(actor->GetId(), "MeshRenderer");
			//actorManager->AddComponentToActor(actor->GetId(), "Material");
		//	auto matContext = MaterialContexts::GetDefaultMaterialContext(EngineManager::Get()->GetGraphicsDevice());
			/*auto materialWeakPtr = actor->GetComponent<Material>("Material");
			if (!materialWeakPtr.expired())
			{*/
			//auto material = materialWeakPtr.lock();
		/*	material->SetGMat(std::make_shared<GMaterial>(matContext));
			material->SetFloat4(ShaderProperty::ProprtyToID("gDiffuseAlbedo"), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetFloat(ShaderProperty::ProprtyToID("gRoughness"), 0.2f);
			material->SetFloat3(ShaderProperty::ProprtyToID("gFresnelR0"), DirectX::XMFLOAT3(0.05f, 0.05f, 0.05f));*/
			auto meshRendererWeakPtr = actor->GetComponent<MeshRenderer>("MeshRenderer");
			if (!meshRendererWeakPtr.expired())
			{
				auto texid = GUIDHelper::StringToResourceId(TEMP_TEXTURE_RESOURCEID_STR);
				auto texResource=dynamic_cast<TextureResource*> (resourceSystem->GetResourceManager()->GetResource(texid));

				auto matid = GUIDHelper::StringToResourceId(MATERIAL_RESOURCEID_STR);
				auto mat = dynamic_cast<MaterialResource*>(resourceSystem->GetResourceManager()->GetResource(matid));
				auto meshRenderer = meshRendererWeakPtr.lock();
				meshRenderer->SetIsEnable(true);
				meshRenderer->SetMeshFilter();
				mat->GetMaterial()->SetFloat4(ShaderProperty::PropertyToID("gDiffuseAlbedo"), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				mat->GetMaterial()->SetFloat(ShaderProperty::PropertyToID("gRoughness"), 0.2f);
				mat->GetMaterial()->SetFloat3(ShaderProperty::PropertyToID("gFresnelR0"), DirectX::XMFLOAT3(0.05f, 0.05f, 0.05f));
				mat->GetMaterial()->SetTexture(ShaderProperty::PropertyToID("MainTex"), texResource);
				meshRenderer->SetMaterial(0, mat);
				renderSystem->ActivateRenderer(meshRenderer.get());
			}
		}


	}
}