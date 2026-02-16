#include"RootSignature.h"
RootSignature::RootSignature(GraphicsDevice* GDevice, int RootCount):
	mSlotRootPrameter(RootCount),
	mDescriptorRangeDatas(RootCount)
{
	mGDevice = GDevice;
	mRootCount = RootCount;
	
}
void RootSignature::AddConstantBufferView()
{
	//if (mCurrentRootParameterIndex < mRootCount)
	//{

		CD3DX12_ROOT_PARAMETER param;
		param.InitAsConstantBufferView(mCurrentCBVBuildIndex++);
		mSlotRootPrameter[mCurrentRootParameterIndex++] = param;
	//}
}
void RootSignature::AddShaderResourceView()
{
	CD3DX12_ROOT_PARAMETER param;
	param.InitAsShaderResourceView(mCurrentSRVBuildIndex++);
	mSlotRootPrameter[mCurrentRootParameterIndex++] = param;
}
void RootSignature::AddSrvDescriptorTable(int numDescriptor)
{
	if (mCurrentRootParameterIndex < mRootCount)
	{
		auto index = mCurrentRootParameterIndex++;
		auto tableRangeIndex = mTableRangeIndex++;
		CD3DX12_ROOT_PARAMETER param = {};
		mDescriptorRangeDatas[tableRangeIndex] .Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, numDescriptor, mSrvDescriptorTableBaseShaderRegisterIndex);
		param.InitAsDescriptorTable(1, &(mDescriptorRangeDatas[tableRangeIndex]), D3D12_SHADER_VISIBILITY_PIXEL);
		mSlotRootPrameter[index] = param;
		mSrvDescriptorTableBaseShaderRegisterIndex += numDescriptor;
	}
}
ComPtr<ID3D12RootSignature> RootSignature::Build()
{
	if (mIsBuild)
	{
		return mRootSig;
	}
	auto staticSamplers = GetStaticSamplers();
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
		(UINT)mSlotRootPrameter.size()
		, mSlotRootPrameter.data(),staticSamplers.size()
		, staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	ComPtr<ID3DBlob>serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowGraphicsfFailed(hr);
	ComPtr<ID3D12RootSignature> mRootSignature;
	ThrowGraphicsfFailed(mGDevice->mD3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf())
	));
	mRootSig = mRootSignature;
	mIsBuild = true;
	mDescriptorRangeDatas.clear();
	return mRootSignature;
}
ID3D12RootSignature* RootSignature::GetRootSig()
{
	return mRootSig.Get();
}

 std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> RootSignature::GetStaticSamplers()
{
	 const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		 0, // shaderRegister
		 D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	 const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		 1, // shaderRegister
		 D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	 const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		 2, // shaderRegister
		 D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	 const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		 3, // shaderRegister
		 D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	 const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		 4, // shaderRegister
		 D3D12_FILTER_ANISOTROPIC, // filter
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		 D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		 0.0f,                             // mipLODBias
		 8);                               // maxAnisotropy

	 const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		 5, // shaderRegister
		 D3D12_FILTER_ANISOTROPIC, // filter
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		 0.0f,                              // mipLODBias
		 8);                                // maxAnisotropy
	 const CD3DX12_STATIC_SAMPLER_DESC shadow(
		 6, // shaderRegister
		 D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, // filter
		 D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressU
		 D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressV
		 D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressW
		 0.0f,                               // mipLODBias
		 16,                                 // maxAnisotropy
		 D3D12_COMPARISON_FUNC_LESS_EQUAL,
		 D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK);
	 return {
		 pointWrap, pointClamp,
		 linearWrap, linearClamp,
		 anisotropicWrap, anisotropicClamp ,shadow };
}