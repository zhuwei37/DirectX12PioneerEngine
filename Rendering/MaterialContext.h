#pragma once
#include"ShaderProperties.h"
#include<DirectXMath.h>
#include "ShaderResourceView.h"
#include"RenderShader.h"
class MaterialContext

{
public:
	MaterialContext(
		std::shared_ptr<RenderShader> renderShader,
		 std::shared_ptr<ShaderProperties>,
		Descriptor);
	//Pass

	//Root
	void SetFloat(ShaderPropertyID propertyId, float value);
	void SetFloat3(ShaderPropertyID propertyId, DirectX::XMFLOAT3 value);
	void SetFloat4(ShaderPropertyID propertyId, DirectX::XMFLOAT4 value);

	bool GetFloat(ShaderPropertyID propertyId, float* value);
	bool GetFloat3(ShaderPropertyID propertyId, DirectX::XMFLOAT3* value);
	bool GetFloat4(ShaderPropertyID propertyId, DirectX::XMFLOAT4* value);


	void SetTexture(ShaderPropertyID propertyId,Texture* tex);
	//std::string GetPropertyValueStr(int propertyId);
	
	void SetGraphicsRoot(ID3D12GraphicsCommandList* commandList);


	std::shared_ptr<RenderShader> mRenderShader;
	std::shared_ptr<ShaderProperties> mShaderProperties;
	Descriptor mTextureDescriptor;

	~MaterialContext();
private:


};