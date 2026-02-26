#pragma once
#include"GraphicsDevice.h"
#include"GTexture.h"
#include"Descriptor.h"
#include"RenderTargetView.h"
#include"MaterialContexts.h"
#include"ShaderResourceView.h"
#include"DefaultConstants.h"
#include"UploadData.h"
class SSAOController
{
public:
	SSAOController(UINT width,UINT height ,
		GraphicsDevice* device);
	void Resize(UINT Width, 
		UINT height);
	void BeginSSAORender(ID3D12GraphicsCommandList* mCommandList
		);
	void OnRenderSSAO(ID3D12GraphicsCommandList* mCommandList,D3D12_GPU_DESCRIPTOR_HANDLE normalMap,
		D3D12_GPU_DESCRIPTOR_HANDLE depthMap);
	void EndSSAORender(ID3D12GraphicsCommandList* mCommandList);
	void UpdateConstantBuffer(CommonPassConstants&);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSsaoMap();
	static const int MaxBlurRadius = 5;
private:

	void GetOffsetVectors(DirectX::XMFLOAT4 offsets[14])
	{
		std::copy(&mOffsets[0], &mOffsets[14], &offsets[0]);
	}
	std::vector<float> CalcGaussWeights(float sigma)
	{
		float twoSigma2 = 2.0f * sigma * sigma;

		
		int blurRadius = (int)ceil(2.0f * sigma);

		assert(blurRadius <= MaxBlurRadius);

		std::vector<float> weights;
		weights.resize(2 * blurRadius + 1);

		float weightSum = 0.0f;

		for (int i = -blurRadius; i <= blurRadius; ++i)
		{
			float x = (float)i;

			weights[i + blurRadius] = expf(-x * x / twoSigma2);

			weightSum += weights[i + blurRadius];
		}

		
		for (int i = 0; i < weights.size(); ++i)
		{
			weights[i] /= weightSum;
		}

		return weights;
	}


	void BuildOffsetVectors();

	void BeginSSAOBlurRender(ID3D12GraphicsCommandList* mCommandList);
	void EndSSAOBlurRender(ID3D12GraphicsCommandList* mCommandList);
	

	UINT mWidth;
	UINT mHeight;
	/// <summary>
	/// AOÍ¼
	/// </summary>
	void BuildAOResource();
	/// <summary>
	/// Ëæ»úÏòÁ¿
	/// </summary>
	void BuildRandomVectorResource();
	void BuildRandomVectorDescriptor();
	void BuildDescriptor();
	GraphicsDevice* mGDevice;
	Descriptor mRtvDescriptors[2];
	Descriptor mSrvDescriptors[3];

	int AoMap1_SrvIndex = 0;
	int AoMap2_SrvIndex = 1;
	int RandomVector_SrvIndex = 2;

	int AoMap1_RtvIndex = 0;
	int AoMap2_RtvIndex = 1;


	//GraphicsResource* mDepthResource;
	std::shared_ptr<GTexture> mRandomVectorMap;
	std::shared_ptr<GTexture> mAOMap1;
	std::shared_ptr<GTexture> mAOMap2;

	std::shared_ptr<RenderTargetView> mAoRtv1;
	std::shared_ptr<RenderTargetView> mAoRtv2;

	std::shared_ptr<ShaderResourceView> mAoSrv1;
	std::shared_ptr<ShaderResourceView> mAoSrv2;
	std::shared_ptr<ShaderResourceView> mRandomVectorSrv;


	std::shared_ptr<MaterialContext> mSsaoMaterialContext;

	SsaoConstants mSsaoCB;

	UploadRenderData<SsaoConstants> mSsaoUploadData;

	DirectX::XMFLOAT4 mOffsets[14];

	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mRect;

	DXGI_FORMAT mAOMapFormat = DXGI_FORMAT_R16_UNORM;
	DXGI_FORMAT mRandomVectorMapFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
};