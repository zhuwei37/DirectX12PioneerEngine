#include<iostream>
#include <boost/algorithm/string/case_conv.hpp>
#include<fstream>
#include"ResourceSystem.h"
#include"FolderProcessor.h"

#include"MeshResource.h"
#include"MeshProcessor.h"
#include"TextureResource.h"
#include"TextureProcessor.h"
#include"EngineManager.h"
#include"MaterialProcessor.h"
#include"ShaderManager.h"
namespace fs = boost::filesystem;
ResourceSystem::ResourceSystem():
	mRootPath(fs::current_path() / fs::path("asset"))

{
	registerProcessor();

	mResourceManager = new ResourceManager();
	registerInMemeryResource();
	init();
}
void ResourceSystem::registerInMemeryResource()
{
	auto device = EngineManager::Get()->GetGraphicsDevice();

	auto boxid = GUIDHelper::StringToResourceId(BOX_MESH_RESOURCEID_STR);
	auto sphereid = GUIDHelper::StringToResourceId(SPHERE_MESH_RESOURCEID_STR);
	auto gridid = GUIDHelper::StringToResourceId(GRID_MESH_RESOURCEID_STR);

	this->ActivityProcessor = ResourceProcessorMap[ResourceType::MESH];

	auto boxMeshResource= std::dynamic_pointer_cast<MeshResource>( this->ActivityProcessor->CreateResource());
	auto sphereMeshResource =std::dynamic_pointer_cast<MeshResource>( this->ActivityProcessor->CreateResource());
	auto gridMeshResource = std::dynamic_pointer_cast<MeshResource>(this->ActivityProcessor->CreateResource());

	boxMeshResource->mId = boxid;
	boxMeshResource->mInMemory = true;
	sphereMeshResource->mId = sphereid;
	sphereMeshResource->mInMemory = true;
	gridMeshResource->mId = gridid;
	gridMeshResource->mInMemory = true;

	std::string boxName = "Box";
	std::string sphereName = "Sphere";
	std::string gridName = "Grid";

	GeometryGenerator geo;
	auto boxMesh=geo.CreateBox(1.0f,1.0f,1.0f,1);
	auto sphereMesh = geo.CreateSphere(0.5f, 20, 20);
	auto gridMesh = geo.CreateGrid(30.0f, 30.0f, 60, 40);

	boxMeshResource->GetMesh()->init(boxMesh, device);
	boxMeshResource->SetName(boxName);
	sphereMeshResource->GetMesh()->init(sphereMesh, device);
	sphereMeshResource->SetName(sphereName);
	gridMeshResource->GetMesh()->init(gridMesh, device);
	gridMeshResource->SetName(gridName);

	std::string boxPath = "BoxMesh";
	std::string spherePath = "SphereMesh";
	std::string gridPath = "GridMesh";

	boxMeshResource->SetPath(boxPath);
	sphereMeshResource->SetPath(spherePath);
	gridMeshResource->SetPath(gridPath);
	





	this->mResourceManager->AddResource(boxMeshResource);
	this->mResourceManager->AddResource(sphereMeshResource);
	this->mResourceManager->AddResource(gridMeshResource);

	


	{
		this->ActivityProcessor = ResourceProcessorMap[ResourceType::TEXTURE];
		auto resource = this->ActivityProcessor->CreateResource();
		auto texResource = std::dynamic_pointer_cast<TextureResource>(resource);
		boost::filesystem::path path = "..\\..\\Textures\\WoodCrate01.dds";
		auto pathws = path.wstring();
		auto texBuffer = GTexture::CreateTextureFromFile(GraphicsDevice::Get(), pathws);
		auto srv = ShaderResourceView::CreateShaderView(GraphicsDevice::Get(), texBuffer.get());
		auto tex = std::make_shared<Texture>(texBuffer, srv);

		auto texid = GUIDHelper::StringToResourceId(TEMP_TEXTURE_RESOURCEID_STR);
		texResource->SetTexture(tex);
		resource->mId = texid;
		resource->mInMemory = true;
		auto pathStr = path.string();
		resource->SetPath(pathStr);

		this->mResourceManager->AddResource(texResource);
	}

	{
		//»·¾³ÌùÍ¼
		auto environmentMapResource = this->ActivityProcessor->CreateResource();
		auto environmentMapTexResource = std::dynamic_pointer_cast<TextureResource>(environmentMapResource);
		boost::filesystem::path environmentMapPath = "..\\..\\Textures\\grasscube1024.dds";
		auto pathws = environmentMapPath.wstring();
		auto environmentMapResourcetexBuffer = GTexture::CreateTextureFromFile(GraphicsDevice::Get(), pathws);
		auto srv = ShaderResourceView::CreateShaderView(GraphicsDevice::Get(), environmentMapResourcetexBuffer.get(), D3D12_SRV_DIMENSION_TEXTURECUBE);
		auto tex = std::make_shared<Texture>(environmentMapResourcetexBuffer, srv);

		auto texid = GUIDHelper::StringToResourceId(DEFAULT_ENVIRONMENT_MAP_TEXTURE_RESOURCEID_STR);
		environmentMapTexResource->SetTexture(tex);
		environmentMapResource->mId = texid;
		environmentMapResource->mInMemory = true;
		auto pathStr = environmentMapPath.string();
		environmentMapResource->SetPath(pathStr);
		this->mResourceManager->AddResource(environmentMapResource);
	}

	{
		this->ActivityProcessor = ResourceProcessorMap[ResourceType::MATERIAL];
		auto mat = this->ActivityProcessor->CreateResource();
		auto  matid = GUIDHelper::StringToResourceId(MATERIAL_RESOURCEID_STR);
		mat->mId = matid;
		mat->mInMemory = true;
		std::string matName = "Default.mat";
		mat->SetPath(matName);
		this->mResourceManager->AddResource(mat);
	}
	{
		this->ActivityProcessor = ResourceProcessorMap[ResourceType::MATERIAL]; 
		auto mat = this->ActivityProcessor->CreateResource();
		auto  matid = GUIDHelper::StringToResourceId(SKYBOX_MATERIAL_RESOURCE_STR);
		mat->mId = matid;
		mat->mInMemory = true;
		std::string matName = "sky.mat";
		mat->SetPath(matName);
		auto rs=std::dynamic_pointer_cast<MaterialResource> (mat);
		//auto RenderShaderID = GUIDHelper::StringToResourceId(SKYBOX_MATERIAL_RESOURCE_STR);
		
		auto texid = GUIDHelper::StringToResourceId(DEFAULT_ENVIRONMENT_MAP_TEXTURE_RESOURCEID_STR);
		auto tex=this->mResourceManager->GetResource(texid);
		auto texPtr = dynamic_cast<TextureResource*>(tex);
		rs->GetMaterial()->SetRenderShader(SKYBOX_RENDER_SHADER_ID);
		rs->GetMaterial()->SetTexture( ShaderProperty::PropertyToID("EnvironmentMapping"), texPtr);
		this->mResourceManager->AddResource(mat);




		auto solidColorMat = this->ActivityProcessor->CreateResource();
		matid = GUIDHelper::StringToResourceId(SOLID_COLOR_MATERIAL_RESOURCEID_STR);
		solidColorMat->mId = matid;
		solidColorMat->mInMemory = true;
		std::string solidColorMatName = "SolidColor.mat";
		solidColorMat->SetPath(solidColorMatName);
		rs = std::dynamic_pointer_cast<MaterialResource>(solidColorMat);
		rs->GetMaterial()->SetRenderShader(SOLIDCOLOR_RENDER_SHADER_ID);
		this->mResourceManager->AddResource(solidColorMat);





	}

	

}
void ResourceSystem::registerProcessor()
{
	//MaterialProcessor m;
	//boost::filesystem::path p = "E:\\BaiduNetdiskDownload\\DxRun\\d3d12book-master\\Chapter 9 Texturing\\Build\\test.mat";
	//m.CreateResource(p);
	ResourceProcessorMap[ResourceType::FOLDER] = new FolderProcessor();
	ResourceProcessorMap[ResourceType::MESH] = new MeshProcessor();
	ResourceProcessorMap[ResourceType::TEXTURE] = new TextureProcessor();
	ResourceProcessorMap[ResourceType::MATERIAL] = new MaterialProcessor();
}
void ResourceSystem::init()
{
	LoadAssetRoot();
}
ResourceManager* ResourceSystem::GetResourceManager()
{
	return mResourceManager;
}
ResourceType ResourceSystem::CheckResourceType(fs::path& path)
{

	ResourceType type = ResourceType::NONE;
	if (fs::is_directory(path))
	{
		type= ResourceType::FOLDER;
		ActivityProcessor = ResourceProcessorMap[type];
		return type;
	}

	auto extension = path.extension().string();
	boost::to_lower(extension);
	if (extension == ".jpg")
	{
		type= ResourceType::TEXTURE;
	}
	else if (extension == ".png")
	{
		type= ResourceType::TEXTURE;
	}
	else if (extension == ".dds")
	{
		type = ResourceType::TEXTURE;
	}
	else if (extension == ".mat")
	{
		type= ResourceType::MATERIAL;
	}
	if (type != ResourceType::NONE)
	{
		ActivityProcessor = ResourceProcessorMap[type];
	}
	return type;
}
//boost::filesystem::path ResourceSystem::GetDescriptionPath(boost::filesystem::path& path)
//{
//	 return =path/ metaExtension
//}
std::shared_ptr<IResource> ResourceSystem::LoadResource(boost::filesystem::path& path)
{
	auto resourceType = CheckResourceType(path);

	if (resourceType != ResourceType::NONE)
	{
		return  ActivityProcessor->LoadResource(path,mResourceManager);
	}
	return nullptr;
}
void ResourceSystem::LoadAssetRoot()
{
	fs::path rootpath = mRootPath;
	if (!fs::is_directory(rootpath))
	{
		if (!fs::create_directory(rootpath))
		{
			return;
		}
	}
	auto pathString = rootpath.string();
	mRootFolder =std::make_shared< FolderResource>(pathString, IResource::NewID());
	mResourceManager->SetRootFolderResource(mRootFolder.get());
	mResourceManager->AddResource(mRootFolder);
	LoadAsset(rootpath, mRootFolder);
	//
		/*auto parentpath=entry.path().parent_path();
		auto joinpath = mRootPath / entry.path();
		auto currentPath = fs::current_path();
		auto filePath = entry.path();
		auto stemName=filePath.stem();
		auto fullpath1 = fs::canonical(filePath);
		auto fullPath2 = fs::absolute(filePath);
		return;*/
	/*for (auto& entry : fs::directory_iterator(rootpath))
	{
		


		if (entry.path().extension() == metaExtension)
		{
			auto absPath=fs::canonical(entry.path());
			IResource* resource= LoadResource(absPath);
			FolderResource* folder= dynamic_cast<FolderResource*>(resource);
			if (folder!=nullptr)
			{

			}
		}
	}*/

}
void ResourceSystem::LoadAsset(boost::filesystem::path& path, std::shared_ptr<FolderResource> parentFolder)
{
	auto parentId = parentFolder->GetId();
	for (auto& entry : fs::directory_iterator(path))
	{
		auto entryPath = entry.path();
		//auto absPath = entryPath;
		//auto r_path=entry.path().relative_path();
		auto ext = entryPath.extension();
		if (ext!= metaExtension)
		{

			auto resource = LoadResource(entryPath);
			if (resource != nullptr)
			{
				std::shared_ptr< FolderResource > folder = std::dynamic_pointer_cast<FolderResource>(resource);
				if (folder != nullptr)
				{
					LoadAsset(entryPath, folder);
				}
				resource->SetParent(parentId);
				parentFolder->AddChildResource(resource->GetId());
				this->mResourceManager->AddResource(resource);
			}
		}
	}
}
ResourceID ResourceSystem::NewID()
{
	return {};
}
bool ResourceSystem::DeleteResource(ResourceID& resourceid,ResourceID& parentid)
{
	auto parentResource = mResourceManager->GetResource(parentid);
	auto resource = mResourceManager->GetResource(resourceid);
	if (parentResource != nullptr)
	{
		parentResource->RemoveChildResource(resourceid);
	}
	if (resource != nullptr)
	{
		if (resource->Children.size() > 0)
		{
			DeleteResourceChildren(resource);
		}
		this->ActivityProcessor = ResourceProcessorMap[resource->mResourceType];
		this->ActivityProcessor->DeleteResource(resource);
		mResourceManager->DeleteResouce(resourceid);
		
	}
	return true;
}
void ResourceSystem::DeleteResourceChildren(IResource*  parent)
{
	for (auto& resourceId : parent->GetChildResources())
	{
		auto resource = mResourceManager->GetResource(resourceId);
		if (resource != nullptr)
		{
			if (resource->Children.size() > 0)
			{
				DeleteResourceChildren(resource);
			}
			this->ActivityProcessor = ResourceProcessorMap[resource->mResourceType];
			this->ActivityProcessor->DeleteResource(resource);
			mResourceManager->DeleteResouce(resourceId);
		}
	}
}
IResource* ResourceSystem::CreateResource(ResourceType type, ResourceID parentId, std::string& pathStr)
{
	if (type == ResourceType::NONE)
	{
		return nullptr;
	}
	this->ActivityProcessor = ResourceProcessorMap[type];
	auto parent = this->mResourceManager->GetResource(parentId);
	if (parent != nullptr)
	{
		boost::filesystem::path p = pathStr;
		auto resource = this->ActivityProcessor->CreateResource(p);
		if (resource != nullptr)
		{

			parent->AddChildResource(resource->GetId());
			resource->SetParent(parentId);
			this->mResourceManager->AddResource(resource);
			return resource.get();;
		}
	}
	return nullptr;
}
ResourceSystem::~ResourceSystem()
{
	delete mResourceManager;
}