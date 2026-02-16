#include"ShaderManager.h"
#include <locale>
#include <codecvt>
ComPtr<ID3DBlob> ShaderManager::LoadShaderData(string filename,string entryPoint,string target)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	auto wFilename = converter.from_bytes(filename);
	auto shader = d3dUtil::CompileShader(wFilename, nullptr, entryPoint, target);
	return shader;
}
void ShaderManager::init()
{
	mShaderDataMap["VS_Default"] = LoadShaderData("..\\..\\Shaders\\Default.hlsl", "VS", "vs_5_0");
	mShaderDataMap["PS_Default"] = LoadShaderData("..\\..\\Shaders\\Default.hlsl", "PS", "ps_5_0");
//mShaderDataMap["VS_SolidColor"] = LoadShaderData("..\\..\\Shaders\\SolidColor.hlsl", "VS", "vs_5_0");
	mShaderDataMap["PS_SolidColor"] = LoadShaderData("..\\..\\Shaders\\SolidColor.hlsl", "PS", "ps_5_0");
	mShaderDataMap["VS_SkyBox"] = LoadShaderData("..\\..\\Shaders\\SkyBox.hlsl", "VS", "vs_5_0");
	mShaderDataMap["PS_SkyBox"] = LoadShaderData("..\\..\\Shaders\\SkyBox.hlsl", "PS", "ps_5_0");

	mShaderDataMap["VS_Deferred"] = LoadShaderData("..\\..\\Shaders\\DefaultVS.hlsl", "VSMain", "vs_5_0");
	mShaderDataMap["PS_Deferred"] = LoadShaderData("..\\..\\Shaders\\DeferredPS.hlsl", "PSMain", "ps_5_0");

	mShaderDataMap["VS_ScreenQuad"] = LoadShaderData("..\\..\\Shaders\\ScreenQuadVS.hlsl", "VSMain", "vs_5_0");
	mShaderDataMap["PS_ScreenQuad"] = LoadShaderData("..\\..\\Shaders\\ScreenQuadPS.hlsl", "PSMain", "ps_5_0");


	mShaderDataMap["PS_DeferredLight"] = LoadShaderData("..\\..\\Shaders\\DeferredLightPS.hlsl", "PSMain", "ps_5_0");


	mShaderDataMap["PS_DeferredPBR"] = LoadShaderData("..\\..\\Shaders\\PBRDeferredPS.hlsl", "PSMain", "ps_5_0");


	mShaderDataMap["PS_ShadowMap"] = LoadShaderData("..\\..\\Shaders\\ShadowPS.hlsl", "PSMain", "ps_5_0");
	mShaderDataMap["VS_ShadowMap"] = LoadShaderData("..\\..\\Shaders\\ShadowVS.hlsl", "VSMain", "vs_5_0");


	LoadShaders();
	LoadRenderShaders();
}
void ShaderManager::LoadShaders()
{
	shared_ptr<Shaders> defaultShaders = std::make_shared<Shaders>();
	defaultShaders->VS =
	{
		reinterpret_cast<BYTE*>(mShaderDataMap["VS_Default"]->GetBufferPointer()),
		mShaderDataMap["VS_Default"]->GetBufferSize()
	};
	defaultShaders->PS =
	{
		reinterpret_cast<BYTE*>(mShaderDataMap["PS_Default"]->GetBufferPointer()),
		mShaderDataMap["PS_Default"]->GetBufferSize()
	};
	defaultShaders->InputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	mShadersMap[DEFUALT_SHADERS_ID] = defaultShaders;



	{
		shared_ptr<Shaders> solidColorShaders = std::make_shared<Shaders>();
		solidColorShaders->VS =
		{
		reinterpret_cast<BYTE*>(mShaderDataMap["VS_Deferred"]->GetBufferPointer()),
		mShaderDataMap["VS_Deferred"]->GetBufferSize()
		};
		solidColorShaders->PS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["PS_SolidColor"]->GetBufferPointer()),
		mShaderDataMap["PS_SolidColor"]->GetBufferSize()
		};
		solidColorShaders->InputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		mShadersMap[SOLIDCOLOR_SHADER_ID] = solidColorShaders;
	}
#pragma region Deferred 



	{
		std::shared_ptr<Shaders> deferredShaders = std::make_shared<Shaders>();
		deferredShaders->VS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["VS_Deferred"]->GetBufferPointer()),
			mShaderDataMap["VS_Deferred"]->GetBufferSize()
		};

		deferredShaders->PS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["PS_Deferred"]->GetBufferPointer()),
			mShaderDataMap["PS_Deferred"]->GetBufferSize()
		}; 
		deferredShaders->InputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mShadersMap[DEFERRED_SHADER_ID] = deferredShaders;
	}

#pragma endregion


#pragma region SkyBox
	std::shared_ptr<Shaders> skyBoxShaders = std::make_shared<Shaders>();
	skyBoxShaders->VS =
	{
		reinterpret_cast<BYTE*>(mShaderDataMap["VS_SkyBox"]->GetBufferPointer()),
	    mShaderDataMap["VS_SkyBox"]->GetBufferSize()
	};
	skyBoxShaders->PS =
	{
		reinterpret_cast<BYTE*>(mShaderDataMap["PS_SkyBox"]->GetBufferPointer()),
	    mShaderDataMap["PS_SkyBox"]->GetBufferSize()
	};
	skyBoxShaders->InputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	mShadersMap[SKYBOX_SHADER_ID] = skyBoxShaders;
#pragma endregion

#pragma region Screen Quad
	{
		std::shared_ptr<Shaders> screenQuadShaders = std::make_shared<Shaders>();
		screenQuadShaders->VS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["VS_ScreenQuad"]->GetBufferPointer()),
			mShaderDataMap["VS_ScreenQuad"]->GetBufferSize()
		};
		screenQuadShaders->PS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["PS_ScreenQuad"]->GetBufferPointer()),
			mShaderDataMap["PS_ScreenQuad"]->GetBufferSize()
		};
		screenQuadShaders->InputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mShadersMap[SCREEN_QUAD_SHADER_ID] = screenQuadShaders;
	}
#pragma endregion

#pragma region DeferredLight
	{
		std::shared_ptr<Shaders> deferredLigthShaders = std::make_shared<Shaders>();
		deferredLigthShaders->VS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["VS_ScreenQuad"]->GetBufferPointer()),
			mShaderDataMap["VS_ScreenQuad"]->GetBufferSize()
		};
		deferredLigthShaders->PS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["PS_DeferredLight"]->GetBufferPointer()),
			mShaderDataMap["PS_DeferredLight"]->GetBufferSize()
		};
		deferredLigthShaders->InputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mShadersMap[DEFERRED_LIGHT_SHADER_ID] = deferredLigthShaders;
	}
#pragma endregion

#pragma region PBR Deferred Shader
	{
		std::shared_ptr<Shaders> pbr_DeferredShaders = std::make_shared<Shaders>();
		pbr_DeferredShaders->VS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["VS_Deferred"]->GetBufferPointer()),
			mShaderDataMap["VS_Deferred"]->GetBufferSize()
		};
		pbr_DeferredShaders->PS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["PS_DeferredPBR"]->GetBufferPointer()),
			mShaderDataMap["PS_DeferredPBR"]->GetBufferSize()
		};
		pbr_DeferredShaders->InputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mShadersMap[PBR_DEFERRED_SHADER_ID] = pbr_DeferredShaders;
	}
#pragma endregion


#pragma region Shadow Map
	{
		std::shared_ptr<Shaders> shadowMapShaders = std::make_shared<Shaders>();
		shadowMapShaders->VS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["VS_ShadowMap"]->GetBufferPointer()),
			mShaderDataMap["VS_ShadowMap"]->GetBufferSize()
		};
		shadowMapShaders->PS =
		{
			reinterpret_cast<BYTE*>(mShaderDataMap["PS_ShadowMap"]->GetBufferPointer()),
			mShaderDataMap["PS_ShadowMap"]->GetBufferSize()
		};
		shadowMapShaders->InputLayout =
		{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		mShadersMap[SHADOW_MAP_SHADER_ID] = shadowMapShaders;
	}
#pragma endregion






}

void ShaderManager::LoadRenderShaders()
{
	std::vector<DXGI_FORMAT> rtvFormats =
	{
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM
	};
	GraphicsDevice* GDevice = GraphicsDevice::Get();
#pragma region Defualt Render Shader
	
	auto defualtShaders = this->GetShaders(DEFUALT_SHADERS_ID);
	auto mRootSignature = std::make_shared<RootSignature>(GDevice, 5);
	//通用参数
	mRootSignature->AddConstantBufferView();
	mRootSignature->AddConstantBufferView();
	mRootSignature->AddConstantBufferView();

	//自定义参数
	mRootSignature->AddConstantBufferView();
	mRootSignature->AddSrvDescriptorTable(2);
	//

	auto rs = mRootSignature->Build();
	//
	std::shared_ptr<PipelineState>  mPipelineState = std::make_shared<PipelineState>(GDevice);
	//mPipelineState->SetCullMode()
	mPipelineState->Build(mRootSignature, defualtShaders.get());

	std::vector<ShaderPass> passes;
	ShaderPass pass(mPipelineState);
	passes.push_back(pass);


	std::shared_ptr<RenderShader> renderShader = std::make_shared<RenderShader>();
	renderShader->mShaderPasses = passes;
	renderShader->mRootSignature = mRootSignature;

	mRenderShaderMap[DEFUALT_RENDER_SHADER_ID] = renderShader;
#pragma endregion

#pragma region SolidColor Render Shader
	{
		auto solidColorShaders = this->GetShaders(SOLIDCOLOR_SHADER_ID);
		auto mSolidColorRootSignature = std::make_shared<RootSignature>(GDevice, 4);
		//通用参数
		mSolidColorRootSignature->AddConstantBufferView();
		mSolidColorRootSignature->AddConstantBufferView();
		mSolidColorRootSignature->AddConstantBufferView();
		//自定义参数
		mSolidColorRootSignature->AddConstantBufferView();
		//

		mSolidColorRootSignature->Build();
		//
		std::shared_ptr<PipelineState>  mSolidColorPipelineState = std::make_shared<PipelineState>(GDevice);
		mSolidColorPipelineState->SetRtvFormat(rtvFormats);
		mSolidColorPipelineState->Build(mSolidColorRootSignature, solidColorShaders.get());

		std::vector<ShaderPass> solidColorPasses;
		ShaderPass mSolidColorPass(mSolidColorPipelineState);
		solidColorPasses.push_back(mSolidColorPass);


		std::shared_ptr<RenderShader> SolidColorRenderShader = std::make_shared<RenderShader>();
		SolidColorRenderShader->mShaderPasses = solidColorPasses;
		SolidColorRenderShader->mRootSignature = mSolidColorRootSignature;

		mRenderShaderMap[SOLIDCOLOR_RENDER_SHADER_ID] = SolidColorRenderShader;
	}
#pragma endregion

#pragma region SkyBox
	{
		auto skyBoxShaders = this->GetShaders(SKYBOX_SHADER_ID);
		auto skyBoxRootSignature = std::make_shared<RootSignature>(GDevice, 4);
		//通用参数
		skyBoxRootSignature->AddConstantBufferView();
		skyBoxRootSignature->AddConstantBufferView();
		skyBoxRootSignature->AddConstantBufferView();
		//自定义参数
		skyBoxRootSignature->AddSrvDescriptorTable(1);

		skyBoxRootSignature->Build();

		std::shared_ptr<PipelineState>  mSkyBoxPipelineState = std::make_shared<PipelineState>(GDevice);
		mSkyBoxPipelineState->SetCullMode(D3D12_CULL_MODE_FRONT);
		mSkyBoxPipelineState->SetDepthFunc(D3D12_COMPARISON_FUNC_LESS_EQUAL);
		mSkyBoxPipelineState->SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK_ZERO);
		mSkyBoxPipelineState->Build(skyBoxRootSignature, skyBoxShaders.get());



		std::vector<ShaderPass> skyBoxPasses;
		ShaderPass mSkyBoxPass(mSkyBoxPipelineState);
		skyBoxPasses.push_back(mSkyBoxPass);


		std::shared_ptr<RenderShader> skyBoxRenderShader = std::make_shared<RenderShader>();
		skyBoxRenderShader->mShaderPasses = skyBoxPasses;
		skyBoxRenderShader->mRootSignature = skyBoxRootSignature;

		mRenderShaderMap[SKYBOX_RENDER_SHADER_ID] = skyBoxRenderShader;
	}
#pragma endregion

	
#pragma region Deferred
	{
		auto deferredShaders= this->GetShaders(DEFERRED_SHADER_ID);
		auto deferredRootSignature = std::make_shared<RootSignature>(GDevice,5);
		deferredRootSignature->AddConstantBufferView();
		deferredRootSignature->AddConstantBufferView();
		deferredRootSignature->AddConstantBufferView();

		deferredRootSignature->AddConstantBufferView();
		deferredRootSignature->AddSrvDescriptorTable(2);

		deferredRootSignature->Build();
		std::shared_ptr<PipelineState> mdeferredPipelineState = std::make_shared<PipelineState>(GDevice);
		
		mdeferredPipelineState->SetRtvFormat(rtvFormats);
		mdeferredPipelineState->Build(deferredRootSignature, deferredShaders.get());

		std::vector<ShaderPass> deferredPasses;
		ShaderPass mdeferredpass(mdeferredPipelineState);
		deferredPasses.push_back(mdeferredpass);

		std::shared_ptr<RenderShader> deferredRenderShader = std::make_shared<RenderShader>();
		deferredRenderShader->mRootSignature = deferredRootSignature;
		deferredRenderShader->mShaderPasses = deferredPasses;

		mRenderShaderMap[DEFERRED_RENDER_SHADER_ID] = deferredRenderShader;
	}
#pragma endregion
#pragma region Screen Quad
	{
		auto screenQuadShaders = this->GetShaders(SCREEN_QUAD_SHADER_ID);
		auto screenQuadRootSignature = std::make_shared<RootSignature>(GDevice, 1);
		screenQuadRootSignature->AddSrvDescriptorTable(1);
		screenQuadRootSignature->Build();

		auto screenQuadPiplineState = std::make_shared<PipelineState>(GDevice);
		screenQuadPiplineState->SetEnableDepth(false);
		screenQuadPiplineState->SetDsvFormat(DXGI_FORMAT_UNKNOWN);
		screenQuadPiplineState->Build(screenQuadRootSignature, screenQuadShaders.get());

		std::vector<ShaderPass> screenQuadPasses;
		ShaderPass screenQuadPass(screenQuadPiplineState);
		screenQuadPasses.push_back(screenQuadPass);

		std::shared_ptr<RenderShader> screenQuadRenderShader = std::make_shared<RenderShader>();
		screenQuadRenderShader->mRootSignature = screenQuadRootSignature;
		screenQuadRenderShader->mShaderPasses = screenQuadPasses;

		mRenderShaderMap[SCREEN_QUAD_RENDER_SHADER_ID] = screenQuadRenderShader;
	}
#pragma endregion
#pragma region Deferred Light


	{
		auto deferredLightShaders = this->GetShaders(DEFERRED_LIGHT_SHADER_ID);
		auto deferredLightRootSignature = std::make_shared<RootSignature>(GDevice, 5);
		deferredLightRootSignature->AddConstantBufferView();
		deferredLightRootSignature->AddConstantBufferView();
		deferredLightRootSignature->AddConstantBufferView();

		deferredLightRootSignature->AddSrvDescriptorTable(4);
		deferredLightRootSignature->AddSrvDescriptorTable(1);
		deferredLightRootSignature->Build();
		auto deferredLightPiplineState = std::make_shared<PipelineState>(GDevice);
		deferredLightPiplineState->SetEnableDepth(false);
		deferredLightPiplineState->SetDsvFormat(DXGI_FORMAT_UNKNOWN);
		deferredLightPiplineState->Build(deferredLightRootSignature, deferredLightShaders.get());


		std::vector<ShaderPass> deferredLightPasses;
		ShaderPass deferredLightPass(deferredLightPiplineState);
		deferredLightPasses.push_back(deferredLightPass);
		std::shared_ptr<RenderShader> deferredLightRenderShader = std::make_shared<RenderShader>();
		deferredLightRenderShader->mRootSignature = deferredLightRootSignature;
		deferredLightRenderShader->mShaderPasses = deferredLightPasses;

		mRenderShaderMap[DEFERRED_LIGHT_RENDER_SHADER_ID] = deferredLightRenderShader;

	}

#pragma endregion
#pragma region PBR Deferred
	{
		auto pbrDeferredShader = this->GetShaders(PBR_DEFERRED_SHADER_ID);
		auto pbrDeferredRootSignature = std::make_shared<RootSignature>(GDevice,3+1);
		pbrDeferredRootSignature->AddConstantBufferView();
		pbrDeferredRootSignature->AddConstantBufferView();
		pbrDeferredRootSignature->AddConstantBufferView();

		pbrDeferredRootSignature->AddSrvDescriptorTable(4);
		pbrDeferredRootSignature->Build();
		auto pbrDeferredPiplineState = std::make_shared<PipelineState>(GDevice);
		
		pbrDeferredPiplineState->SetRtvFormat(rtvFormats);
		pbrDeferredPiplineState->Build(pbrDeferredRootSignature, pbrDeferredShader.get());

		std::vector<ShaderPass> pbrDeferredPasses;
		ShaderPass pbrDeferredPass(pbrDeferredPiplineState);
		pbrDeferredPasses.push_back(pbrDeferredPass);

		std::shared_ptr<RenderShader> pbrDeferredRenderShader = std::make_shared<RenderShader>();
		pbrDeferredRenderShader->mRootSignature = pbrDeferredRootSignature;
		pbrDeferredRenderShader->mShaderPasses = pbrDeferredPasses;

		mRenderShaderMap[PBR_DEFERRED_RENDER_SHADER_ID] = pbrDeferredRenderShader;
	}
#pragma endregion

#pragma region Shadow Map
	{
		auto shadowMapShader = this->GetShaders(SHADOW_MAP_SHADER_ID);
		auto shadowMapRootSignature = std::make_shared<RootSignature>(GDevice, 3);
		shadowMapRootSignature->AddConstantBufferView();
		shadowMapRootSignature->AddConstantBufferView();
		shadowMapRootSignature->AddConstantBufferView();

		shadowMapRootSignature->Build();

		auto shadowMapPiplineState = std::make_shared<PipelineState>(GDevice);
		shadowMapPiplineState->SetEnableRtv(false);
		shadowMapPiplineState->SetDepthBais(100000, 0.0f, 1.0f);
		shadowMapPiplineState->Build(shadowMapRootSignature, shadowMapShader.get());

		std::vector<ShaderPass> shadowMapPasses;
		ShaderPass shadowMapPass(shadowMapPiplineState);
		shadowMapPasses.push_back(shadowMapPass);

		std::shared_ptr<RenderShader> shadowMapRenderShader = std::make_shared<RenderShader>();
		shadowMapRenderShader->mRootSignature = shadowMapRootSignature;
		shadowMapRenderShader->mShaderPasses = shadowMapPasses;

		mRenderShaderMap[SHADOW_MAP_RENDER_SHADER_ID] = shadowMapRenderShader;
	}
#pragma endregion




}





std::shared_ptr<RenderShader>    ShaderManager::GetRenderShader(int renderShaderId)
{
	auto it = mRenderShaderMap.find(renderShaderId);
	if (it != mRenderShaderMap.end())
	{
		return it->second;
	}
	return nullptr;
}
shared_ptr<Shaders> ShaderManager::GetShaders(string name)
{
	auto it = mShadersMap.find(name);
	if (it != mShadersMap.end())
	{
		return it->second;
	}
	return nullptr;
}