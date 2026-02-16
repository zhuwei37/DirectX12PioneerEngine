#pragma once
#include"MaterialContext.h"
#include"MaterialContexts.h"
#include<memory>
//class GMaterial
//{
//public:
//	GMaterial(std::shared_ptr<MaterialContext>);
//	GMaterial()=default;
//	void SetFloat(int propertyId, float value);
//	void SetFloat3(int propertyId, DirectX::XMFLOAT3 value);
//	void SetFloat4(int propertyId, DirectX::XMFLOAT4 value);
//
//	int GetFloat(int propertyId, float* value);
//	int GetFloat3(int propertyId, DirectX::XMFLOAT3* value);
//	int GetFloat4(int propertyId, DirectX::XMFLOAT4* value);
//
//	void BindMaterialContext(std::shared_ptr<MaterialContext>);
//	MaterialContext* GetMaterialContext();
//private:
//	std::shared_ptr<MaterialContext> mContext;
//};