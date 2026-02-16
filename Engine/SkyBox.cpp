#include"SkyBox.h"
#include"Transform.h"
#include"interface_resource.h"
#include"MeshResource.h"
SkyBox::SkyBox()
{
	this->init();
}
void SkyBox::init()
{
	
	/*auto resourceManager= EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto actorManager = EngineManager::Get()->GetActorManager();
	auto sphereResourceID=GUIDHelper::StringToResourceId(SPHERE_MESH_RESOURCEID_STR);
	auto sphereMeshResource=std::dynamic_pointer_cast<MeshResource>(resourceManager->GetResourceStrongPtr(sphereResourceID));


	mActor = actorManager->CreateActor();
	actorManager->AddComponentToActor(mActor->GetId(), "MeshFilter");
	actorManager->AddComponentToActor(mActor->GetId(), "MeshRenderer");
	auto transfrom = mActor->GetComponent<Transform>("Transform").lock();
	this->transform = transfrom.get();;
	auto meshFiiter = mActor->GetComponent<MeshFilter>("MeshFilter").lock();
	auto meshRenderer = mActor->GetComponent<MeshRenderer>("MeshRenderer").lock();
	meshRenderer->AddMaterial();
	transfrom->SetScale(5000.0f, 5000.0f, 5000.0f);
	meshFiiter->SetMeshResource(sphereMeshResource);
	auto  matid = GUIDHelper::StringToResourceId(SKYBOX_MATERIAL_RESOURCE_STR);
	auto mat=dynamic_cast<MaterialResource*>( resourceManager->GetResource(matid));
	meshRenderer->SetMaterial(0, mat);*/


	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	transform = new Transform();
	auto id = GUIDHelper::StringToResourceId(SPHERE_MESH_RESOURCEID_STR);
	skyBoxMesh =( (MeshResource*)resourceManager->GetResource(id))->GetMesh();
	transform->SetScale(5000.0f, 5000.0f, 5000.0f);
}
void SkyBox::BeginRenderSkyBox()
{

}
void SkyBox::EndRenderSkyBox()
{
}
Mesh* SkyBox::GetMesh()
{
	return skyBoxMesh;
}
SkyBox::~SkyBox()
{
	
}
Transform* SkyBox::Update()
{
	transform->UpdateMatrix();
	return transform;
}