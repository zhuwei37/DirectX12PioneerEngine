#pragma once
#include"IResource.h"
#include"Material.h"
#include<map>
#define TextureStr "Texture"
#define PropertyStr "Property"
#define Float4Str "float4"
#define FloatStr "float"
#define Float3Str "float3"
#include"yaml-cpp/emitter.h"
class MaterialResource :public IResource
{
	
public:
	
	MaterialResource()
	{
		mMat = std::make_shared<Material>();
		this->mResourceType = ResourceType::MATERIAL;
		this->mId = IResource::NewID();
	}
	Material* GetMaterial()
	{
		if (mMat != nullptr)
		{
			return mMat.get();
		}
		return nullptr;
		
	}
	std::string Serialize()override;

	static void SerializeShaderValue(YAML::Emitter &out, IShaderValue* ptr, Material* mat);

private:
	struct Var 
	{
		ShaderPropertyID shaderPropertyID;
		ShaderValueType type;
		union 
		{
			float Float4x4[4][4];
			float Float4[4];
			float Float3[3];
			float Float2[2];
			float Float;
			ResourceID _ResourceID;
		};
	};
private :
	std::shared_ptr<Material> mMat;
	
};