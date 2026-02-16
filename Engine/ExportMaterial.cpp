#include"ExportMaterial.h"
#include"Material.h"
#include"EngineManager.h"
#include"interface_resource.h"
#include"ShaderValue.h"
long long MaterialNameToPropertyID(const char* materialName)
{
	std::string name(materialName);
	return ShaderProperty::PropertyToID(name);
}


int GetMaterialShaderID(ResourceID matResourceID)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			return matResource->GetMaterial()->GetRenderShader();
		}
	}
	return -1;
}

bool SetMaterialShaderID(ResourceID matResourceID, int shaderID)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			matResource->GetMaterial()->SetRenderShader(shaderID);
		}
	}
	return -1;
}

float GetMaterialPropertyFloat(ResourceID matResourceID, long long propertyId)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			float value=0;
			if (matResource->GetMaterial()->GetFloat(propertyId, value))
			{
				return value;
			}
		}
	}
	return 0;
}

DirectX::XMFLOAT3 GetMaterialPropertyFloat3(ResourceID matResourceID, long long propertyId)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			DirectX::XMFLOAT3 value;
			if (matResource->GetMaterial()->GetFloat3(propertyId, value))
			{
				return value;
			}
			 
		}
	}
	return DirectX::XMFLOAT3{0,0,0};
}

DirectX::XMFLOAT4 GetMaterialPropertyFloat4(ResourceID matResourceID, long long propertyId)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			DirectX::XMFLOAT4 value;
			if (matResource->GetMaterial()->GetFloat4(propertyId, value))
			{
				return value;
			}
		}
	}
	return DirectX::XMFLOAT4{0,0,0,0};
}
ResourceID GetMaterialPropertyTexture(ResourceID matResourceID, long long propertyId)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			ResourceID textureResourceID;
			if (matResource->GetMaterial()->GetTexture(propertyId, textureResourceID))
			{
				return textureResourceID;
			}
		}
	}
	return RESOURCEID_NONE;
}

void SetMaterialPropertyFloat(ResourceID matResourceID, long long propertyId, float refValue)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			matResource->GetMaterial()->SetFloat(propertyId, refValue);
		}
	}
	//return false;
}
void SetMaterialPropertyFloat3(ResourceID matResourceID, long long propertyId, DirectX::XMFLOAT3 refValue)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			matResource->GetMaterial()->SetFloat3(propertyId, refValue);
		}
	}
	//return -1;
}
void SetMaterialPropertyFloat4(ResourceID matResourceID, long long propertyId, DirectX::XMFLOAT4 refValue)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			matResource->GetMaterial()->SetFloat4(propertyId, refValue);
		}
	}
	//return -1;
}
void SetMaterialPropertyTexture(ResourceID matResourceID, long long propertyId, ResourceID textureResourceID)
{
	auto resourceManager = EngineManager::Get()->GetResourceSystem()->GetResourceManager();
	auto resource = resourceManager->GetResource(matResourceID);
	if (resource != nullptr)
	{
		auto matResource = dynamic_cast<MaterialResource*>(resource);
		if (matResource != nullptr)
		{
			auto resource2=resourceManager->GetResource(textureResourceID);
			auto textureResource = dynamic_cast<TextureResource*>(resource2);
			if (textureResource != nullptr)
			{

				matResource->GetMaterial()->SetTexture(propertyId,textureResource);
			}
			else if (textureResourceID == RESOURCEID_NONE)
			{
				matResource->GetMaterial()->SetTexture(propertyId, nullptr);
			}
			
		}
	}
	//return -1;
}