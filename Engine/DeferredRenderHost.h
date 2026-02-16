#pragma once
#include"GraphicsDevice.h"
#include"interface_resource.h"
#include"Descriptor.h"
#include"RenderHost.h"
#include"MaterialContexts.h"
#include"ShadowMapController.h"
#include"ShadowMap.h"
class DeferredRenderHost:public RenderHost
{

public:
	enum  GBufferRTVOrder : int
	{
		/// <summary>
		/// 世界坐标 材质处理ID，根据材质ID进行相应处理
		/// </summary>
		WorldPosAndMatIndex=0,
		/// <summary>
		/// 法线和粗糙度
		/// </summary>
		NormalAndRoughness,
		/// <summary>
		/// 漫反射光与金属度
		/// </summary>
		AlbedoAndMetallic,
		/// <summary>
		/// 镜面反射与自发光或者菲
		/// </summary>
		ApecularAndEmissive,

		GBufferLength,
		//光照
		Light= GBufferLength,
		Length,
	};
	DXGI_FORMAT MRT_Formats[GBufferRTVOrder::Length]
		= { 
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM,
	};

	DXGI_FORMAT LightRtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM;


	DeferredRenderHost(GraphicsDevice* GDevice, CreateRenderHostParameter* param);
	~DeferredRenderHost();

	 void Resize(int,int)override;
	 ComPtr<ID3D12GraphicsCommandList>  OnRender()override;
	 //const float ClearColor[4] = {1,}
private:
	void CreateScreenQuadMesh();
	void CreateMRTResource();
	void CreateMRTDescriptor();
	void MRTResourceBindDescriptor();
	void RenderShadowMap();
	void RenderGBuffer();
	void RenderLight();
	void RenderSkyBox();
	void RenderWorldPosQuad();
	void PreRender(std::map<Layer, RendererList>& renderers, SkyBox* skybox)override;



	void DrawMesh(Mesh* mesh);
	//int mWidth;
	//int mHeight;
	//GraphicsDevice* mGDevice;
	Descriptor MRT_SrvDescriptorHandle;
	Descriptor MRT_RtvDescriptorHandle;
	std::shared_ptr<GTexture> MRTResources[GBufferRTVOrder::Length];
	
	//延迟渲染流水线
	std::shared_ptr<MaterialContext> deferredMaterialContext;
	std::shared_ptr<MaterialContext> screenQuadMaterialContext;
	std::shared_ptr<MaterialContext> deferredLightMaterialContext;


	std::shared_ptr<MaterialContext> shadowMapMaterialContext;


	std::shared_ptr<MaterialContext> skyBoxMaterialContext;

	Material* mSkyBoxMat;
	//std::shared_ptr<M>

	std::shared_ptr<Mesh> woroldPosQuadMesh;
	std::shared_ptr<Mesh> normalRoughnessQuadMesh;
	std::shared_ptr<Mesh> diffuseQuadMesh;
	std::shared_ptr<Mesh> dpecularAndEmissiveQuadMesh;
	std::shared_ptr<Mesh> lightQuadMesh;

	std::shared_ptr<Mesh> ScreenQuadMesh;
	//std::shared_ptr<Mesh>
	std::shared_ptr<SkyBox> mSkybox;


private:


	void UpdateShadowTransform();
	void UpdateCommonShadowPassConstants();
	UploadRenderData<CommonPassConstants> mCommonShadowPassConstantsRenderData;

	std::shared_ptr<ShadowMapController> mShadowMapController;
	std::shared_ptr<ShadowMap> mShadowMap;

	DirectX::XMFLOAT3 mLightPosW;
	DirectX::XMFLOAT4X4 mLightView;
	DirectX::XMFLOAT4X4 mLightProj;
	DirectX::XMFLOAT4X4 mShadowTransform;

	DirectX::BoundingSphere mSceneBounds;
	
	
};