#pragma once
#include"interface.h"
#include"IComponent.h"
#include"MaterialContext.h"
#include"DirectXMath.h"
#include<map>
#include"TextureResource.h"
class Material
{

public:
	enum MaterialPrepertyType:int
	{
		Float,
		Float2,
		Float3,
		Float4,
		Float3x3,
		Float4x4,
		Int,
		Bool,
		_Texture,
	};
	struct  MaterialPropertyData
	{
		std::string PropertyName;
		std::string PropertyValue;
		MaterialPrepertyType PropertyType;
	};


	Material();
	
	void SetGMat(std::shared_ptr<MaterialContext> mat);
	MaterialContext* GetGMat();
	//属性
	bool GetFloat(ShaderPropertyID propertyId,float& refValue);
	bool GetFloat3(ShaderPropertyID porpertyId, DirectX::XMFLOAT3& refValue);
	bool GetFloat4(ShaderPropertyID proeprtyId, DirectX::XMFLOAT4& refValue);

	void SetFloat(ShaderPropertyID propertyId,float value );
	void SetFloat3(ShaderPropertyID propertyId, DirectX::XMFLOAT3 value);
	void SetFloat4(ShaderPropertyID propertyId,DirectX::XMFLOAT4 value);
	
	void SetTexture(ShaderPropertyID proeprtyId,TextureResource* texResource);
	bool GetTexture(ShaderPropertyID porpertyId, ResourceID& refValue);
	//需要知道shader id
	void SetRenderShader(ShaderPropertyID renderShaderId);
	int GetRenderShader();
	//纹理

private:
	std::map<int, ResourceID> mMaterialPropertyTextureResourceIDMap;
	int mRenderShaderId=0;
	std::shared_ptr<MaterialContext> mGMat;
	
};