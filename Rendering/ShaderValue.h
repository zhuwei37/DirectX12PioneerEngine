#pragma once
#include"d3dx12.h"
#include"UploadData.h"
#include<map>
#include<unordered_map>
#include<memory>
#include<vector>
#include"RootSignature.h"
#include"PipelineState.h"
#include"ShaderResourceView.h"
#include"Texture.h"
#include <functional>  

typedef long long ShaderPropertyID;
enum ShaderValueType:int
{
	Float,
	Float2,
	Float3,
	Float4,
	Float3x3,
	Float4x4,
	Int,
	Bool,
	_Texture,
};
class ShaderProperty
{
public:
	static ShaderPropertyID PropertyID;


	static const char* PropertyIDToString(ShaderPropertyID id)
	{
		auto findIt = PropertyIDToStrMap.find(id);
		if (findIt != PropertyIDToStrMap.end())
		{
			return findIt->second.c_str();
		}
		return nullptr;
	}
	static ShaderPropertyID PropertyToID(std::string property)
	{

		auto it = PropertyIDMap.find(property);
		if (it != PropertyIDMap.end())
		{
			return it->second;
		}
		else
		{
			ShaderPropertyID newID = PropertyID++;
			PropertyIDMap[property] = newID;
			PropertyIDToStrMap[newID] = property;
			return newID;
		}
	}
private:
	static std::unordered_map<std::string, ShaderPropertyID>   PropertyIDMap;
	static std::map<ShaderPropertyID,std::string> PropertyIDToStrMap;
};
class IShaderValue
{
public:
	IShaderValue() {}
	virtual ShaderValueType GetShaderValueType() = 0;
	virtual ShaderPropertyID GetShaderPropertyID() { return this->ShaderPropertyID; }
protected:
	ShaderPropertyID ShaderPropertyID;
	
};
class ShaderTextureValue :public IShaderValue
{
public:
	ShaderValueType GetShaderValueType()
	{
		return ShaderValueType::_Texture;
	}
	
	ShaderTextureValue(GraphicsDevice* gDevice,Descriptor descriptor,int textureInDescriptorIndex) 
	{
		this->mGDevice = gDevice;
		this->mTextureDescriptor = descriptor;
		this->mTextureInDescirptorIndex = textureInDescriptorIndex;
	}
	void SetTexture(Texture* tex)
	{
		if (mTextureDescriptor.Number > mTextureInDescirptorIndex)
		{
			if (tex != nullptr)
			{

				auto srv = tex->GetShaderResourceView();
				auto srcCpuHandle = srv->GetCPUView();

				CD3DX12_CPU_DESCRIPTOR_HANDLE   destCpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mTextureDescriptor.CpuHandle);
				destCpuHandle.Offset(mTextureInDescirptorIndex, mGDevice->CBV_SRV_UAV_DescriptorSize);
				//拷贝到可见堆中
				mGDevice->mD3dDevice->CopyDescriptorsSimple
				(
					1, destCpuHandle, srcCpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
				);
			}
			else
			{
				auto defualtTex=Texture::GetDefualtTexture();
				CD3DX12_CPU_DESCRIPTOR_HANDLE   destCpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mTextureDescriptor.CpuHandle);
				destCpuHandle.Offset(mTextureInDescirptorIndex, mGDevice->CBV_SRV_UAV_DescriptorSize);
				//拷贝到可见堆中
				mGDevice->mD3dDevice->CopyDescriptorsSimple
				(
					1, destCpuHandle, defualtTex->GetShaderResourceView()->GetCPUView(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
				);
			}
		}
		
	}
private:
	/// <summary>
	/// 纹理描述符堆 
	/// </summary>
	Descriptor mTextureDescriptor;
	GraphicsDevice* mGDevice;
	int mTextureInDescirptorIndex;
};
template<typename T>
class ShaderValue :public IShaderValue
{
public :
	

	ShaderValueType GetShaderValueType()
	{
		return mType;
	}
	T GetValue()
	{
		return mValue;
	}
	void SetValue(T value)
	{
		mValue = value;
		if (mUploadData != nullptr)
		{
			BYTE* data = reinterpret_cast<BYTE*>(&mValue);
				mUploadData->SetData(data, mOffset, sizeof(T));
		}
	}
	
	ShaderValue(UploadGraphicsData* uploadData,T defaultValue,UINT offset,ShaderValueType type) :
		mUploadData(uploadData),
		mOffset(offset),
		mType(type)
	{
		SetValue(defaultValue);
	}
	ShaderValue(UINT offset, ShaderValueType type) :

		mOffset(offset)

	{
		mType = type;
	}
	void SetUploadGraphicsData(UploadGraphicsData* uploadData)
	{
		mUploadData = uploadData;
	}
private:
	 UploadGraphicsData* mUploadData;
	 T mValue;
	 ShaderValueType mType;
     UINT mOffset;
};

class ShaderPass
{
	
	//渲染流水线
public :
	ShaderPass() {}
	ShaderPass(std::shared_ptr<PipelineState>  Pipline)
	{
		mPipline = (Pipline);
	}
	PipelineState* GetPipline()
	{
		if (mPipline != nullptr)
		{
			return mPipline.get();
		}
		return nullptr;
	}
private:
	std::shared_ptr<PipelineState> mPipline;
};



//template<typename T>
//class IShaderValue
//{
//public:
//	virtual ShaderValueType GetShaderValueType()override
//	{
//		return mType;
//	}
//	virtual T GetValue() = 0;
//	virtual void SetValue(T) = 0;
//	void SetValue(T& value)
//	{
//		SetValue(value);
//	}
//protected:
//	ShaderValueType mType;
//};
//class ShaderFloatValue :public IShaderValue<float>
//{
//public:
//	ShaderFloatValue(UploadGraphicsData* uploadData, float value, UINT offset)
//		:mUploadData(uploadData),
//
//		mOffset(offset)
//	{
//		SetValue(value);
//	}
//
//	virtual float GetValue() override
//	{
//		return mValue;
//	}
//	virtual void SetValue(float value)override
//	{
//		mValue = value;
//		if (mUploadData != NULL)
//		{
//			BYTE* data = reinterpret_cast<BYTE*>(&mValue);
//			mUploadData->SetData(data, mOffset, 4);
//		}
//	}
//private:
//	UploadGraphicsData* mUploadData;
//	float mValue;
//	UINT mOffset;
//};
//class ShaderFloat2Value :public IShaderValue<DirectX::XMFLOAT2>
//{
//public:
//	ShaderFloat2Value(UploadGraphicsData* uploadData, DirectX::XMFLOAT2 value, UINT offset, ShaderValueType type)
//		:mUploadData(uploadData),
//
//		mOffset(offset)
//	{
//		mType = type;
//		SetValue(value);
//	}
//
//	virtual DirectX::XMFLOAT2 GetValue()
//	{
//		return mValue;
//	}
//	virtual void SetValue(DirectX::XMFLOAT2 value)
//	{
//		mValue = value;
//		if (mUploadData != NULL)
//		{
//			BYTE* data = reinterpret_cast<BYTE*>(&mValue);
//			mUploadData->SetData(data, mOffset, sizeof(DirectX::XMFLOAT2));
//		}
//	}
//private:
//	UploadGraphicsData* mUploadData;
//	DirectX::XMFLOAT2 mValue;
//	UINT mOffset;
//};
//class ShaderMartrix4x4Value:public IShaderValue<>
