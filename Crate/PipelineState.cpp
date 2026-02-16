#include"PipelineState.h"

PipelineState::PipelineState(GraphicsDevice* GDevice)
	:mGDevice(GDevice)
{
	mDepthStencilFormat = GDevice->DepthStencilFormat;
}
RootSignature* PipelineState::GetRootSignature()
{
	return mRootSignature.get();
}
void PipelineState::SetCullMode(D3D12_CULL_MODE mode)
{
	this->mCullMode = mode;
}
void PipelineState::SetDepthFunc(D3D12_COMPARISON_FUNC depthFunc)
{
	this->mDepthFunc = depthFunc;
}
void PipelineState::SetRtvFormat(std::vector<DXGI_FORMAT> &rtvFormat)
{
	if (rtvFormat.size() > 8)
	{
		return;
	}
	this->RTVFormat = rtvFormat;
}
void PipelineState::SetEnableRtv(bool flag)
{
	this->mEnableRtv = flag;
}
void PipelineState::SetDepthBais(int depthBais, float depthBiasClamp, float slopeScaleDepthBias)
{
	this->mDepthBias = depthBais;
	this->mDepthBiasClamp = depthBiasClamp;
	this->mSlopeScaledDepthBias = slopeScaleDepthBias;
}
void PipelineState::SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK mask)
{
	this->mDepthWriteMask = mask;
}
void PipelineState::SetDsvFormat(DXGI_FORMAT format)
{
	this->mDepthStencilFormat = format;
}
void PipelineState::SetEnableDepth(bool flag)
{
	this->mEnableDepth = flag;
}
void PipelineState::Build(std::shared_ptr<RootSignature> rootSignature, Shaders* shaders)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

	mRootSignature = rootSignature;


	ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { shaders->InputLayout.data(), (UINT)shaders->InputLayout.size() };
	opaquePsoDesc.pRootSignature = rootSignature.get()->GetRootSig();
	opaquePsoDesc.VS = shaders->VS;

	opaquePsoDesc.PS = shaders->PS;

	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.RasterizerState.CullMode = this->mCullMode;

	opaquePsoDesc.RasterizerState.DepthBias = this->mDepthBias;
	opaquePsoDesc.RasterizerState.DepthBiasClamp = this->mDepthBiasClamp;
	opaquePsoDesc.RasterizerState.SlopeScaledDepthBias = this->mSlopeScaledDepthBias;

	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	opaquePsoDesc.DepthStencilState.DepthEnable = this->mEnableDepth;


	opaquePsoDesc.DepthStencilState.DepthFunc = this->mDepthFunc;
	opaquePsoDesc.DepthStencilState.DepthWriteMask = this->mDepthWriteMask;
	opaquePsoDesc.DSVFormat = this->mDepthStencilFormat;



	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	if (mEnableRtv)
	{

		if (RTVFormat.size() > 0)
		{
			opaquePsoDesc.NumRenderTargets = RTVFormat.size();
			for (int i = 0; i < RTVFormat.size(); i++)
			{
				opaquePsoDesc.RTVFormats[i] = RTVFormat[i];
			}
		}
		else
		{
			opaquePsoDesc.NumRenderTargets = 1;
			opaquePsoDesc.RTVFormats[0] = mGDevice->BackBufferFormat;
		}
	}
	else
	{
		opaquePsoDesc.NumRenderTargets = 0;
		opaquePsoDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	}


	opaquePsoDesc.SampleDesc.Count = 1;
	opaquePsoDesc.SampleDesc.Quality = 0;

	ThrowGraphicsfFailed(mGDevice->mD3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&this->mDxPipelineState)));


}