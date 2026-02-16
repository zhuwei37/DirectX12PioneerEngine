#include"Material.h"
#include"MaterialContexts.h"
#include"TextureResource.h"
#include"interface_resource.h"
Material::Material()
{
	SetRenderShader(mRenderShaderId);
}

void Material::SetGMat(std::shared_ptr<MaterialContext> gMat)
{
	this->mGMat = gMat;
}
MaterialContext* Material::GetGMat()
{
	return mGMat.get();
}
void Material::SetFloat(ShaderPropertyID propertyId, float value)
{
	if (mGMat != nullptr)
	{
		mGMat->SetFloat(propertyId, value);
	}
}
void Material::SetFloat3(ShaderPropertyID propertyId, DirectX::XMFLOAT3 value)
{
	if (mGMat != nullptr)
	{
		mGMat->SetFloat3( propertyId, value);
	}
}
void Material::SetFloat4(ShaderPropertyID propertyId, DirectX::XMFLOAT4 value)
{
	if (mGMat != nullptr)
	{
		mGMat->SetFloat4(propertyId, value);
	}
}
void Material::SetTexture(ShaderPropertyID propertyId, TextureResource* texResource)
{
	if (mGMat != nullptr)
	{
		if (texResource != nullptr)
		{

			mGMat->SetTexture(propertyId, texResource->GetTexture());
			mMaterialPropertyTextureResourceIDMap[propertyId] = texResource->GetId();
		}
		else
		{
			mGMat->SetTexture(propertyId, nullptr);
			mMaterialPropertyTextureResourceIDMap[propertyId] = RESOURCEID_NONE;
		}
	}
	else
	{
	}
}
bool Material::GetFloat(ShaderPropertyID propertyId, float& refValue)
{
	if (mGMat != nullptr)
	{
		return mGMat->GetFloat(propertyId, &refValue);
	}
	return false;
}
bool Material::GetFloat3(ShaderPropertyID propertyId, DirectX::XMFLOAT3& refValue)
{
	if (mGMat != nullptr)
	{
		return mGMat->GetFloat3(propertyId, &refValue);
	}
	return false;
}
bool  Material::GetFloat4(ShaderPropertyID propertyId, DirectX::XMFLOAT4& refValue)
{
	if (mGMat != nullptr)
	{
		return mGMat->GetFloat4(propertyId, &refValue);
	}
	return false;
}
bool Material::GetTexture(ShaderPropertyID propertyId, ResourceID& textureResourceID)
{
	auto findIt = mMaterialPropertyTextureResourceIDMap.find(propertyId);
	if (findIt != mMaterialPropertyTextureResourceIDMap.end())
	{
		textureResourceID = findIt->second;
		return true;
	}
	return false;
}
void Material::SetRenderShader(ShaderPropertyID renderShaderId)
{
	if (renderShaderId >= RENDER_SHADER_ID_MIN && renderShaderId <= RENDER_SHADER_ID_MAX)
	{
		
		mRenderShaderId = renderShaderId;
		mGMat = MaterialContexts::GetMaterialContext(renderShaderId);
		mMaterialPropertyTextureResourceIDMap.clear();
	}
}
int Material::GetRenderShader()
{
	return mRenderShaderId;
}