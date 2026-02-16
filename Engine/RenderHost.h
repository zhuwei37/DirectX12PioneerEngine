#pragma once
#include"SwapChainGraphicsPresenter.h"
#include"CameraController.h"
#include<memory>
#include"interface.h"
#include"DepthStencilResourceView.h"
#include"UploadData.h"
#include"DefualtLight.h"
#include"DefaultConstants.h"
#include"Renderer.h"
#include"Actor.h"
#include"SkyBox.h"
using namespace std;
using RendererList = std::vector<Renderer*>;
class RenderHost
{
public:
	RenderHost(GraphicsDevice* GDevice, CreateRenderHostParameter* param);
	CameraController* GetCameraController();
	virtual void Resize(int width, int Height);
	virtual void PreRender(std::map<Layer, RendererList>&,SkyBox* );
	virtual ComPtr<ID3D12GraphicsCommandList>  OnRender();
	bool IsVisible();
	void SetVisible(bool );
	void AddRenderer(Renderer* renderer);
	void RemoveRenderer(Renderer* renderer);
	void Present();


	virtual ~RenderHost() {}

	UploadRenderData<CommonPassConstants> mCommonPassConstantsRenderData;
	UploadRenderData<CommonLight> mCommonLightRenderData;
	UploadRenderData<ObjectPassConstants> mCommonObjectPassContantsRenderData;
protected:
	void UpdateLight();
	void UpdateCommonPassConstants();
	void UpdateObjectPassConstants();
	void initialization();
	//void initSky();
	shared_ptr<DepthStencilResourceView> mDsv;
	shared_ptr<CameraController> mCamera;
	shared_ptr<SwapChainGraphicsPresenter> mPresenter;
	std::shared_ptr<GTexture> mDepthStencilGraphicsResource;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;

	CommonPassConstants mCommonPass;
	CommonLight mCommonLight;



	std::vector<Renderer*> mRenderers;
	std::vector<Renderer*> mActivityRenderers;
	int objIndex = 0;
	int mSkyObjIndex = 0;
	SkyBox* mSkyBox;

protected:


	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mRect;
	GraphicsDevice* mGDevice;
	int mWidth;
	int mHeight;
	bool mIsVisible;
};