#pragma once
#include"IResource.h"
#include"Texture.h"
class TextureResource :public IResource
{
public:
	TextureResource(std::string& path, std::string& name, ResourceID& id, std::shared_ptr<Texture> tex)
	{
		this->mPath = path;
		this->mName = name;
		this->mId = id;
		this->mTex = tex;
		this->mResourceType = ResourceType::TEXTURE;
	}
	TextureResource()
	{
	
		this->mId = IResource::NewID();;
		this->mResourceType = ResourceType::TEXTURE;
	}
	void SetTexture(std::shared_ptr<Texture> tex)
	{
		this->mTex = tex;
	}
	Texture* GetTexture()
	{
		return mTex.get();
	}
private:
	std::shared_ptr<Texture> mTex;
};