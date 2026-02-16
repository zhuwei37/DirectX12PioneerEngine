#pragma once
#include "d3dUtil.h"

class Filter
{
public:
	///<summary>
	/// The width and height should match the dimensions of the input texture to blur.
	/// Recreate when the screen is resized. 
	///</summary>
	Filter(ID3D12Device* device,
		UINT width, UINT height,
		DXGI_FORMAT format);

	Filter(const Filter& rhs) = delete;
	Filter& operator=(const Filter& rhs) = delete;
	~Filter() = default;

	ID3D12Resource* Output();

	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor,
		CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor,
		UINT descriptorSize);

	void OnResize(UINT newWidth, UINT newHeight);
	void SaveToPNG();
	///<summary>
	/// Blurs the input texture blurCount times.
	///</summary>
	void Execute(
		ID3D12GraphicsCommandList* cmdList,
		ID3D12Resource* input);

private:
	

	void BuildDescriptors();
	void BuildResources();

private:

	const int MaxBlurRadius = 5;

	ID3D12Device* md3dDevice = nullptr;

	UINT mWidth = 0;
	UINT mHeight = 0;
	DXGI_FORMAT mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur0CpuSrv;



	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuSrv;



	// Two for ping-ponging the textures.
	Microsoft::WRL::ComPtr<ID3D12Resource> mBlurMap0 = nullptr;
};
