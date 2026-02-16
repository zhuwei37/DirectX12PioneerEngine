#pragma once
#include"ShaderValue.h"
#include"MaterialContext.h"
class RenderContextGenerator
{

public:
	RenderContextGenerator(GraphicsDevice* gDevice):
		mGDevice(gDevice)
	{
	}
	std::unique_ptr<MaterialContext> GetRenderContext()
	{
		return std::move(mDefaultRenderContext);
	}
	void CreateDefault()
	{
		if (mDefaultRenderContext != nullptr)
		{
			return;
		}
		ShaderFactory factory;
		factory.SetShader(ShaderType::VS, "..\\..\\Shaders\\Default.hlsl", "VS", "vs_5_0");
		factory.SetShader(ShaderType::PS, "..\\..\\Shaders\\Default.hlsl", "PS", "ps_5_0");
		auto shaders = factory.ToShaders();

		auto mRootSignature = std::make_shared<RootSignature>(mGDevice, 4);
		std::shared_ptr<PipelineState>  mPipelineState = std::make_shared<PipelineState>(mGDevice);


		mRootSignature->AddConstantBufferView();
		mRootSignature->AddConstantBufferView();
		mRootSignature->AddConstantBufferView();
		mRootSignature->AddSrvDescriptorTable(1);
		auto rs = mRootSignature->Build();

		mPipelineState->Build(mRootSignature, shaders.get());

		std::map<int, IShaderValue*> shaderValueMap;
		UINT len = sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT3) + sizeof(float) ;
		std::shared_ptr<UploadGraphicsData> data = std::make_shared<UploadGraphicsData>(mGDevice, len, 1);
		int id;
		UINT offset = 0;

		id = ShaderProperty::ProprtyToID("gDiffuseAlbedo");
		ShaderValue<DirectX::XMFLOAT4>* DiffuseAlbedon = new  ShaderValue<DirectX::XMFLOAT4>(offset, ShaderValueType::Float4);
		DiffuseAlbedon->SetUploadGraphicsData(data.get());
		offset += sizeof(DirectX::XMFLOAT4);
		shaderValueMap[id] = DiffuseAlbedon;

		id = ShaderProperty::ProprtyToID("gFresnelR0");
		ShaderValue<DirectX::XMFLOAT3>* FresnelR0 = new ShaderValue<DirectX::XMFLOAT3>(offset, ShaderValueType::Float3);
		FresnelR0->SetUploadGraphicsData(data.get());
		offset += sizeof(DirectX::XMFLOAT3);
		shaderValueMap[id] = FresnelR0;


		id = ShaderProperty::ProprtyToID("gRoughness");
		ShaderValue<float>* Roughness = new ShaderValue<float>(offset, ShaderValueType::Float);
		Roughness->SetUploadGraphicsData(data.get());
		offset += sizeof(float);
		shaderValueMap[id] = Roughness;

		/*id = ShaderProperty::ProprtyToID("gMatTransform");
		ShaderValue<DirectX::XMFLOAT4X4>* MatTransform = new ShaderValue<DirectX::XMFLOAT4X4>(offset, ShaderValueType::Float4x4);
		MatTransform->SetUploadGraphicsData(data.get());*/
		//shaderValueMap[id] = MatTransform;


		std::vector<ShaderPass> passes;
		ShaderPass pass(mPipelineState);
		passes.push_back(pass);
		std::shared_ptr<ShaderProperties> ShaderPropertiess = std::make_shared<ShaderProperties>(data, shaderValueMap);
		mDefaultRenderContext = std::make_unique<MaterialContext>(passes, mRootSignature, ShaderPropertiess);
	}
private :
	GraphicsDevice* mGDevice;
	std::unique_ptr<MaterialContext> mDefaultRenderContext;

};