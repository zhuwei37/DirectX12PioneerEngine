//***************************************************************************************
// BlurFilter.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Filter.h"
#include"d3d12.h"/*
#include <d3d11on12.h>*/
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

Filter::Filter(ID3D12Device* device,
	UINT width, UINT height,
	DXGI_FORMAT format)
{
	md3dDevice = device;

	mWidth = width;
	mHeight = height;
	mFormat = format;

	BuildResources();
}

ID3D12Resource* Filter::Output()
{
	return mBlurMap0.Get();
}

void Filter::BuildDescriptors(CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor,
	CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor,
	UINT descriptorSize)
{
	// Save references to the descriptors. 
	mBlur0CpuSrv = hCpuDescriptor;
	

	mBlur0GpuSrv = hGpuDescriptor;


	BuildDescriptors();
}

void Filter::OnResize(UINT newWidth, UINT newHeight)
{
	if ((mWidth != newWidth) || (mHeight != newHeight))
	{
		mWidth = newWidth;
		mHeight = newHeight;

		BuildResources();

		// New resource, so we need new descriptors to that resource.
		BuildDescriptors();
	}
}

void Filter::Execute(ID3D12GraphicsCommandList* cmdList,
	
	ID3D12Resource* input
	)
{
	//cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
	//	D3D12_RESOURCE_STATE_COMMON,
	//	D3D12_RESOURCE_STATE_COPY_DEST));
	//Copy
	auto desc=input->GetDesc();
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	UINT numRows;
	UINT64 rowSize, totalBytes;
	md3dDevice->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &numRows, &rowSize, &totalBytes);
	CD3DX12_TEXTURE_COPY_LOCATION dstLocation(mBlurMap0.Get(), footprint);
	CD3DX12_TEXTURE_COPY_LOCATION srcLocation(input, 0);

	cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);



	SaveToPNG();

	//cmdList->CopyTextureRegion
	/*cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_COMMON
	));*/
}

void Filter::SaveToPNG()
{
	void* pData = nullptr;

	mBlurMap0->Map(0, nullptr, &pData);

	
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width = mWidth;
	desc.Height = mHeight;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = mFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 1;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrint;
	UINT numRows;
	UINT64 rowSize, totalBytes;
	md3dDevice->GetCopyableFootprints(&desc, 0, 1, 0, &footPrint, nullptr, nullptr, nullptr);
	std::vector<uint8_t> imageData(mWidth * mHeight * 4);
	const uint8_t* sourceData = reinterpret_cast<const uint8_t*>(pData);
	for (UINT y = 0; y < mHeight; y++)
	{
		for (UINT x = 0; x < mWidth; x++)
		{
			const UINT srcIndex = y * footPrint.Footprint.RowPitch + x * 4;
			const UINT dstIndex = (y * mWidth + x) * 4;
			imageData[dstIndex + 0] = sourceData[srcIndex + 0];
			imageData[dstIndex + 1] = sourceData[srcIndex + 1];
			imageData[dstIndex + 2] = sourceData[srcIndex + 2];
			imageData[dstIndex + 3] = sourceData[srcIndex + 3];
		}
	}
	

	mBlurMap0->Unmap(0, nullptr);
	std::string fileName = ".\\img.png";
	int result = stbi_write_png(
		fileName.c_str(),
		mWidth,
		mHeight,
		4,
		imageData.data(),
		mWidth * 4
	);
	if (result != 0)
	{
	}

}
void Filter::BuildDescriptors()
{
	/*D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = mFormat;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	md3dDevice->CreateShaderResourceView(mBlurMap0.Get(), &srvDesc, mBlur0CpuSrv);*/
	
}

void Filter::BuildResources()
{
	
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;;
	texDesc.Alignment = 0;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = mFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	UINT64 bufferSize = 0;



	md3dDevice->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &bufferSize);

	ThrowIfFailed(md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&mBlurMap0)));

	;
}