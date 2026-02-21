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
	friend class MaterialProcessor;
public:
	
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
public:
	
	MaterialResource()
	{
		//mMat = std::make_shared<Material>();
		this->mResourceType = ResourceType::MATERIAL;
		this->mId = IResource::NewID();
	}
	MaterialResource(ResourceID id)
	{
		//mMat = std::make_shared<Material>();
		this->mResourceType = ResourceType::MATERIAL;
		this->mId = id;
	}
	Material* GetMaterial()
	{
		if (mMat == nullptr)
		{
			this->mMat = std::make_shared<Material>();
			SetValues(this->mMat.get());
		}
		return mMat.get();
		
	}

	std::string Serialize()override;

	static void SerializeShaderValue(YAML::Emitter &out, IShaderValue* ptr, Material* mat);


	
private :

	void SetValues(Material* mat);
	std::shared_ptr<Material> mMat;
	std::vector<Var> mTempVars;
	int mTempRenderShaderId = 0;
};