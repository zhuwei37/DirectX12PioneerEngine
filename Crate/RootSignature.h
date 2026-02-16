#pragma once
#include"GraphicsException.h"
#include"GraphicsDevice.h"
#include<vector>
/// <summary>
/// ¸ùÇ©Ãû
/// </summary>
class RootSignature
{
public:
	RootSignature(GraphicsDevice* GDevice,int RootCount);
	ComPtr<ID3D12RootSignature> Build();
	void AddSrvDescriptorTable(int numDescriptor);
	void AddUavDescriptorTable(int numDescriptor) = delete;
	void AddCbvDescriptorTable() = delete;
	void AddConstantBufferView();
	void AddShaderResourceView();
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();
	ID3D12RootSignature* GetRootSig();
private:
	int mCurrentCBVBuildIndex=0;
	int mCurrentSRVBuildIndex = 0;
	int mCurrentRootParameterIndex=0;
	int mSrvDescriptorTableBaseShaderRegisterIndex = 0;
	int mTableRangeIndex = 0;
	int mRootCount;
	bool mIsBuild = false;

	

	GraphicsDevice* mGDevice;
	ComPtr<ID3D12RootSignature> mRootSig;
	std::vector<CD3DX12_ROOT_PARAMETER> mSlotRootPrameter;
	std::vector<CD3DX12_DESCRIPTOR_RANGE> mDescriptorRangeDatas;
};