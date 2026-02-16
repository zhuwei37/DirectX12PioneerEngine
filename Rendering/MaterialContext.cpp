
#include"MaterialContext.h"
MaterialContext::MaterialContext(std::shared_ptr<RenderShader> rs
	,std::shared_ptr<ShaderProperties> properties,Descriptor textureDescriptor )
	
{
	this->mTextureDescriptor = textureDescriptor;
	this->mRenderShader = rs;
	this->mShaderProperties = properties;
}
void MaterialContext::SetTexture(ShaderPropertyID propertyId,Texture* tex)
{
	if (mShaderProperties != nullptr)
	{
		mShaderProperties->SetValue(propertyId, tex);
	}
}
void MaterialContext::SetFloat(ShaderPropertyID propertyId, float value)
{
	if (mShaderProperties != nullptr)
	{
		mShaderProperties->SetValue<float>(propertyId, value, ShaderValueType::Float);
	}
}
void MaterialContext:: SetFloat3(ShaderPropertyID propertyId, DirectX::XMFLOAT3 value)
{
	if (mShaderProperties != nullptr)
	{
		mShaderProperties->SetValue<DirectX::XMFLOAT3>(propertyId, value, ShaderValueType::Float3);
	}
}
void MaterialContext::SetFloat4(ShaderPropertyID propertyId, DirectX::XMFLOAT4 value)
{
	if (mShaderProperties != nullptr)
	{
		mShaderProperties->SetValue<DirectX::XMFLOAT4>(propertyId, value, ShaderValueType::Float4);
	}
}

bool MaterialContext::GetFloat(ShaderPropertyID propertyId, float* value)
{
	if (mShaderProperties != nullptr)
	{
		return mShaderProperties->GetValue<float>(propertyId, value, ShaderValueType::Float)==0;
	}
	return -1;
}
bool MaterialContext::GetFloat3(ShaderPropertyID propertyId, DirectX::XMFLOAT3* value)
{
	if (mShaderProperties != nullptr)
	{
		return mShaderProperties->GetValue<DirectX::XMFLOAT3>(propertyId, value, ShaderValueType::Float3)==0;
	}
	return -1;
}
bool MaterialContext::GetFloat4(ShaderPropertyID propertyId, DirectX::XMFLOAT4* value)
{
	if (mShaderProperties != nullptr)
	{
		return mShaderProperties->GetValue<DirectX::XMFLOAT4>(propertyId, value, ShaderValueType::Float4)==0;
	}
	return -1;
}
void MaterialContext::SetGraphicsRoot(ID3D12GraphicsCommandList* commandList)
{
	int rootParameterIndex = 3;
	if (mShaderProperties->HasCustomData())
	{
		commandList->SetGraphicsRootConstantBufferView(rootParameterIndex++ , mShaderProperties->mUploadGraphicsData->NavigateResource()->GetGPUVirtualAddress());
	}
	if (mTextureDescriptor.Number > 0)
	{

		commandList->SetGraphicsRootDescriptorTable(rootParameterIndex++, mTextureDescriptor.GpuHandle);
	}
}
MaterialContext::~MaterialContext()
{

}
ShaderPropertyID ShaderProperty::PropertyID = 0;
std::unordered_map<std::string, ShaderPropertyID>  ShaderProperty::PropertyIDMap;
std::map<ShaderPropertyID, std::string> ShaderProperty::PropertyIDToStrMap;