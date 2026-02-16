#pragma once
#include"GraphicsDevice.h"
#include"RootSignature.h"
#include"ShaderFactory.h"
/// <summary>
/// äÖÈ¾Á÷Ë®Ïß
/// </summary>
class PipelineState
{
public :
	
public:
	PipelineState(GraphicsDevice* GDevice);
	void Build(std::shared_ptr<RootSignature>,Shaders*);
	void SetCullMode(D3D12_CULL_MODE mode);
	void SetDepthFunc(D3D12_COMPARISON_FUNC depthFunc);
	void SetRtvFormat(std::vector<DXGI_FORMAT> &rtvFormats);
	void SetEnableRtv(bool flag);
	void SetDepthBais(int depthBais,float depthBiasClamp,float slopeScaleDepthBias);
	void SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK mask);
	void SetEnableDepth(bool flag);
	void SetDsvFormat(DXGI_FORMAT format);
	RootSignature* GetRootSignature();
	ComPtr<ID3D12PipelineState> mDxPipelineState;
private:
	/// <summary>
	/// ÌÞ³ý
	/// </summary>
	D3D12_CULL_MODE mCullMode= D3D12_CULL_MODE_BACK;
	D3D12_COMPARISON_FUNC mDepthFunc= D3D12_COMPARISON_FUNC_LESS;
	DXGI_FORMAT mDepthStencilFormat;
	bool mEnableDepth=true;
	D3D12_DEPTH_WRITE_MASK mDepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	GraphicsDevice* mGDevice;
	std::shared_ptr<RootSignature> mRootSignature;


	bool mEnableRtv = true;

	int mDepthBias = 0;
	float mDepthBiasClamp = 0;
	float mSlopeScaledDepthBias = 0;

	std::vector<DXGI_FORMAT> RTVFormat;
};