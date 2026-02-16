#pragma once
#include"GraphicsDevice.h"
#include"GraphicsResource.h"
#include"d3dUtil.h"
template<typename T>
class UploadRenderData :public GraphicsResource
{
public:

	UploadRenderData(GraphicsDevice* GDevice, UINT elementCount, bool isConstantBuffer)
		:mIsContantBuffer(isConstantBuffer)
	{
		mElementByteSize = sizeof(T);
		if (isConstantBuffer)
		{
			mElementByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(T));
		}
		auto property=CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto bufferDesc=CD3DX12_RESOURCE_DESC::Buffer(elementCount * mElementByteSize);
		ThrowGraphicsfFailed(
			GDevice->mD3dDevice->CreateCommittedResource
			(
				&property,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&Resource)
			)
		);
		ThrowGraphicsfFailed(Resource->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
		//memset(mMappedData, 0, elementCount * mElementByteSize);
	}
	UploadRenderData(const UploadRenderData& rhs) = delete;
	UploadRenderData& operator=(const UploadRenderData& rhs) = delete;
	~UploadRenderData()
	{
		if (Resource != nullptr)
		{
			Resource->Unmap(0, nullptr);
		}
		Resource = nullptr;
	}
	void SetData(int elementIndex, const T& data)
	{
		memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
	}
private:
	BYTE* mMappedData=nullptr;
	UINT mElementByteSize = 0;
	bool mIsContantBuffer = false;
};

class UploadGraphicsData :public GraphicsResource
{
public:

	UploadGraphicsData(GraphicsDevice* GDevice,UINT ElementByteSize, UINT elementCount, bool isConstantBuffer=false)
		:mIsContantBuffer(isConstantBuffer)
	{
		mElementByteSize = ElementByteSize;
		if (isConstantBuffer)
		{
			mElementByteSize = d3dUtil::CalcConstantBufferByteSize(ElementByteSize);
		}
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(elementCount * mElementByteSize);
		auto property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ThrowGraphicsfFailed(
			GDevice->mD3dDevice->CreateCommittedResource
			(
				&property,
				D3D12_HEAP_FLAG_NONE,
				&buffer,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&Resource)
			)
		);
		ThrowGraphicsfFailed(Resource->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
	}
	UploadGraphicsData(const UploadGraphicsData& rhs) = delete;
	UploadGraphicsData& operator=(const UploadGraphicsData& rhs) = delete;
	~UploadGraphicsData()
	{
		if (Resource != nullptr)
		{
			Resource->Unmap(0, nullptr);
		}
		Resource = nullptr;
	}
	void SetData(int elementIndex, const BYTE* data,UINT size)
	{
		memcpy(&mMappedData[elementIndex * mElementByteSize], data, size);
	}
	void SetData(const BYTE* data,UINT offset, UINT size)
	{
		memcpy(&mMappedData[offset], data, size);
	}
	void SetDataByOffset(int elementIndex,const BYTE* data,UINT size)
	{
	}
private:
	BYTE* mMappedData = nullptr;
	UINT mElementByteSize = 0;
	bool mIsContantBuffer = false;
};