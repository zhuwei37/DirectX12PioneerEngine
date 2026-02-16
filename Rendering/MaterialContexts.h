#pragma once

#include"MaterialContext.h"
#include"ShaderManager.h"
#include<memory>
#include <ShaderResourceView.h>
class MaterialContexts
{
public:
	static std::shared_ptr<MaterialContext> GetMaterialContext(int renderShaderId)
	{
		GraphicsDevice* dev = GraphicsDevice::Get();
		switch (renderShaderId)
		{
		case DEFUALT_RENDER_SHADER_ID:
		{
			return GetDefaultMaterialContext(dev);
		}
		case SOLIDCOLOR_RENDER_SHADER_ID:
		{
			return GetSolidColorMaterialContext(dev);
		}
		case SKYBOX_RENDER_SHADER_ID:
		{
			return GetSkyBoxMaterialContext(dev);
		}
		case DEFERRED_RENDER_SHADER_ID:
		{
			return GetDeferredMaterialContext(dev);
		}
		case SCREEN_QUAD_RENDER_SHADER_ID:
		{
			return GetScreenQuadMaterialContext(dev);
		}
		case DEFERRED_LIGHT_RENDER_SHADER_ID:
		{
			return GetDeferredLightMaterialContext(dev);
		}
		case PBR_DEFERRED_RENDER_SHADER_ID:
		{
			return GetPBRDeferredMaterialContext(dev);
		}
		case SHADOW_MAP_RENDER_SHADER_ID:
		{
			return GetShadowMapMaterialContext(dev);
		}
		default:
			break;
		}
		return nullptr;
	}
private:
	static shared_ptr<MaterialContext> GetDefaultMaterialContext(GraphicsDevice * mGDevice)
	{
		std::map<ShaderPropertyID, IShaderValue*> shaderValueMap;
		UINT len = sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT3) + sizeof(float) ;
		std::shared_ptr<UploadGraphicsData> data = std::make_shared<UploadGraphicsData>(mGDevice, len, 1);
		ShaderPropertyID id;
		UINT offset = 0;

		id = ShaderProperty::PropertyToID("gDiffuseAlbedo");
		ShaderValue<DirectX::XMFLOAT4>* DiffuseAlbedon = new  ShaderValue<DirectX::XMFLOAT4>(offset, ShaderValueType::Float4);
		DiffuseAlbedon->SetUploadGraphicsData(data.get());
		offset += sizeof(DirectX::XMFLOAT4);
		shaderValueMap[id] = DiffuseAlbedon;

		id = ShaderProperty::PropertyToID("gFresnelR0");
		ShaderValue<DirectX::XMFLOAT3>* FresnelR0 = new ShaderValue<DirectX::XMFLOAT3>(offset, ShaderValueType::Float3);
		FresnelR0->SetUploadGraphicsData(data.get());
		offset += sizeof(DirectX::XMFLOAT3);
		shaderValueMap[id] = FresnelR0;


		id = ShaderProperty::PropertyToID("gRoughness");
		ShaderValue<float>* Roughness = new ShaderValue<float>(offset, ShaderValueType::Float);
		Roughness->SetUploadGraphicsData(data.get());
		offset += sizeof(float);
		shaderValueMap[id] = Roughness;
		auto rs = ShaderManager::Get()->GetRenderShader(DEFERRED_RENDER_SHADER_ID);
		Descriptor d = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(2);

		ShaderTextureValue* stv = new ShaderTextureValue(mGDevice, d, 0);
		id = ShaderProperty::PropertyToID("MainTex");
		shaderValueMap[id] = stv;

		ShaderTextureValue* normalTex = new ShaderTextureValue(mGDevice, d, 1);
		id = ShaderProperty::PropertyToID("NormalTex");
		shaderValueMap[id] = normalTex;

		std::shared_ptr<ShaderProperties> ShaderPropertiess = std::make_shared<ShaderProperties>(data, shaderValueMap);
		auto mDefaultRenderContext = std::make_shared<MaterialContext>(rs, ShaderPropertiess,d);
		
		//mDefaultRenderContext->mSrv = std::move(mSrv);

		return mDefaultRenderContext;
	}
	static shared_ptr<MaterialContext> GetSolidColorMaterialContext(GraphicsDevice* mGDevice)
	{
		std::map<ShaderPropertyID, IShaderValue*> shaderValueMap;
		UINT len = sizeof(DirectX::XMFLOAT4) ;
		std::shared_ptr<UploadGraphicsData> data = std::make_shared<UploadGraphicsData>(mGDevice, len, 1);
		ShaderPropertyID id;
		UINT offset = 0;

		id = ShaderProperty::PropertyToID("gColor");
		ShaderValue<DirectX::XMFLOAT4>* gColor = new  ShaderValue<DirectX::XMFLOAT4>(offset, ShaderValueType::Float4);
		gColor->SetUploadGraphicsData(data.get());
		offset += sizeof(DirectX::XMFLOAT4);
		shaderValueMap[id] = gColor;
		DirectX::XMFLOAT4 value = { 0.2f,0.5f ,0.8f ,1.0f };
		gColor->SetValue(value);
		
		auto rs = ShaderManager::Get()->GetRenderShader(SOLIDCOLOR_RENDER_SHADER_ID);
	
		
		std::shared_ptr<ShaderProperties> ShaderPropertiess = std::make_shared<ShaderProperties>(data, shaderValueMap);
		auto mSoliColorRenderContext = std::make_shared<MaterialContext>(rs, ShaderPropertiess, Descriptor{0});

		//mDefaultRenderContext->mSrv = std::move(mSrv);

		return mSoliColorRenderContext;
	}
	static shared_ptr<MaterialContext> GetSkyBoxMaterialContext(GraphicsDevice* mGDevice)
	{
		std::map<ShaderPropertyID, IShaderValue*> shaderValueMap;

		
		auto rs = ShaderManager::Get()->GetRenderShader(SKYBOX_RENDER_SHADER_ID);
		Descriptor d = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(1);
		ShaderTextureValue* stv = new ShaderTextureValue(mGDevice, d, 0);
		ShaderPropertyID skyMapid = ShaderProperty::PropertyToID("EnvironmentMapping");
		shaderValueMap[skyMapid] = stv;


		std::shared_ptr<ShaderProperties> ShaderPropertiess = std::make_shared<ShaderProperties>(nullptr, shaderValueMap);
		auto RenderContext = std::make_shared<MaterialContext>(rs, ShaderPropertiess, d);

		//mDefaultRenderContext->mSrv = std::move(mSrv);

		return RenderContext;
	}
	static std::shared_ptr<MaterialContext> GetDeferredMaterialContext(GraphicsDevice* mGDevice)
	{
		std::map<ShaderPropertyID, IShaderValue*> shaderValueMap;
		UINT len = sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT3) + sizeof(float);
		std::shared_ptr<UploadGraphicsData> data = std::make_shared<UploadGraphicsData>(mGDevice, len, 1);
		ShaderPropertyID id;
		UINT offset = 0;

		id = ShaderProperty::PropertyToID("gDiffuseAlbedo");
		ShaderValue<DirectX::XMFLOAT4>* DiffuseAlbedon = new  ShaderValue<DirectX::XMFLOAT4>(offset, ShaderValueType::Float4);
		DiffuseAlbedon->SetUploadGraphicsData(data.get());
		offset += sizeof(DirectX::XMFLOAT4);
		shaderValueMap[id] = DiffuseAlbedon;

		id = ShaderProperty::PropertyToID("gFresnelR0");
		ShaderValue<DirectX::XMFLOAT3>* FresnelR0 = new ShaderValue<DirectX::XMFLOAT3>(offset, ShaderValueType::Float3);
		FresnelR0->SetUploadGraphicsData(data.get());
		offset += sizeof(DirectX::XMFLOAT3);
		shaderValueMap[id] = FresnelR0;


		id = ShaderProperty::PropertyToID("gRoughness");
		ShaderValue<float>* Roughness = new ShaderValue<float>(offset, ShaderValueType::Float);
		Roughness->SetUploadGraphicsData(data.get());
		offset += sizeof(float);
		shaderValueMap[id] = Roughness;
		Descriptor d = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(2);

		ShaderTextureValue* stv = new ShaderTextureValue(mGDevice, d, 0);
		id = ShaderProperty::PropertyToID("MainTex");
		shaderValueMap[id] = stv;

		ShaderTextureValue* normalTex = new ShaderTextureValue(mGDevice, d, 1);
		id = ShaderProperty::PropertyToID("NormalTex");
		shaderValueMap[id] = normalTex;

		auto rs = ShaderManager::Get()->GetRenderShader(DEFERRED_RENDER_SHADER_ID);
		std::shared_ptr<ShaderProperties> ShaderPropertiess = std::make_shared<ShaderProperties>(data, shaderValueMap);

		auto RenderContext = std::make_shared<MaterialContext>(rs, ShaderPropertiess,d);
		return RenderContext;
	}
	static std::shared_ptr<MaterialContext> GetScreenQuadMaterialContext(GraphicsDevice* mGDevice)
	{
		auto rs = ShaderManager::Get()->GetRenderShader(SCREEN_QUAD_RENDER_SHADER_ID);
		auto renderContext = std::make_shared<MaterialContext>(rs, nullptr, Descriptor{ 0 });
		return renderContext;
	}
	static std::shared_ptr<MaterialContext> GetDeferredLightMaterialContext(GraphicsDevice* mGDevice)
	{
		auto rs = ShaderManager::Get()->GetRenderShader(DEFERRED_LIGHT_RENDER_SHADER_ID);
		auto renderContext = std::make_shared<MaterialContext>(rs, nullptr, Descriptor{ 0 });
		return renderContext;
	}
	static std::shared_ptr<MaterialContext> GetShadowMapMaterialContext(GraphicsDevice* mGDevice)
	{
		auto rs = ShaderManager::Get()->GetRenderShader(SHADOW_MAP_RENDER_SHADER_ID);
		auto renderContext = std::make_shared<MaterialContext>(rs, nullptr, Descriptor{ 0 });
		return renderContext;
	}
	static std::shared_ptr<MaterialContext> GetPBRDeferredMaterialContext(GraphicsDevice* mGDevice)
	{
		auto rs = ShaderManager::Get()->GetRenderShader(PBR_DEFERRED_RENDER_SHADER_ID);
		std::map<ShaderPropertyID, IShaderValue*> shaderValueMap;
		auto d = mGDevice->CBV_SRV_UAV_Shader_Visible_DescriptorHeapAllocator->Allocator(4);
		
		ShaderTextureValue* baseColorTex = new ShaderTextureValue(mGDevice, d, 0);
		ShaderPropertyID propertyId = ShaderProperty::PropertyToID("BaseColorTex");
		shaderValueMap[propertyId] = baseColorTex;

		ShaderTextureValue* normalTex = new ShaderTextureValue(mGDevice, d, 1);
		propertyId = ShaderProperty::PropertyToID("NormalTex");
		shaderValueMap[propertyId] = normalTex;

		ShaderTextureValue* metallicTex = new ShaderTextureValue(mGDevice, d, 2);
		propertyId = ShaderProperty::PropertyToID("MetallicTex");
		shaderValueMap[propertyId] = metallicTex;

		ShaderTextureValue* roughness = new ShaderTextureValue(mGDevice, d, 3);
		propertyId = ShaderProperty::PropertyToID("RoughnessTex");
		shaderValueMap[propertyId] = roughness;


		std::shared_ptr<ShaderProperties> ShaderPropertiess = std::make_shared<ShaderProperties>(nullptr, shaderValueMap);
		auto renderContext = std::make_shared<MaterialContext>(rs, ShaderPropertiess,d);
		return renderContext;
	}
};