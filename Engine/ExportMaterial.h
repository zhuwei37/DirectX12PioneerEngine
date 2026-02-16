#pragma once
#pragma once
#include "ExPort.h"
#include"ResourceSystem.h"
#include"interface.h"
extern "C"
{

	DLL_EXPORTS long long MaterialNameToPropertyID(const char* MaterialName);


	DLL_EXPORTS int GetMaterialShaderID(ResourceID matResourceID);
	DLL_EXPORTS bool SetMaterialShaderID(ResourceID matResourceID, int shaderID);

	DLL_EXPORTS float GetMaterialPropertyFloat(ResourceID matResourceID, long long propertyId);
	DLL_EXPORTS DirectX::XMFLOAT3 GetMaterialPropertyFloat3(ResourceID matResourceID, long long propertyId);
	DLL_EXPORTS DirectX::XMFLOAT4 GetMaterialPropertyFloat4(ResourceID matResourceID, long long propertyId);
	DLL_EXPORTS ResourceID GetMaterialPropertyTexture(ResourceID matResourceID, long long proeprty);

	DLL_EXPORTS void SetMaterialPropertyFloat(ResourceID matResourceID,long long propertyId,float refValue);
	DLL_EXPORTS void SetMaterialPropertyFloat3(ResourceID matResourceID, long long propertyId,DirectX::XMFLOAT3 refValue);
	DLL_EXPORTS void SetMaterialPropertyFloat4(ResourceID matResourceID, long long propertyId,DirectX::XMFLOAT4 refValue);
	DLL_EXPORTS void SetMaterialPropertyTexture(ResourceID matResourceID, long long propertyId,ResourceID textureResourceID);

}