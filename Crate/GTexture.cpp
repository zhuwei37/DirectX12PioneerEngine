#include"GTexture.h"
#include <locale>
#include <codecvt>
GTexture::GTexture(ComPtr<ID3D12Resource> resource)
{
	Resource = resource;
}
UINT GTexture::GetHeight()const
{
	if (Resource != nullptr)
	{
		auto Height = Resource->GetDesc().Height;
		return Height;
	}
	return 0;
}
UINT GTexture::GetWidth()const
{

	if (Resource != nullptr)
	{
		auto Width = Resource->GetDesc().Width;
		return Width;
	}
	return 0;
}

std::shared_ptr<GTexture> GTexture::CreateRenderTargetTexture(GraphicsDevice* gDevice, ResourceDescription resourceDesc)
{
	D3D12_RESOURCE_DESC renderTargetDesc = {};
	renderTargetDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	renderTargetDesc.Alignment = 0;
	renderTargetDesc.Width = resourceDesc.Width;
	renderTargetDesc.Height = resourceDesc.Height;
	renderTargetDesc.DepthOrArraySize = 1;
	renderTargetDesc.MipLevels = 1;
	renderTargetDesc.Format = resourceDesc.Format;

	renderTargetDesc.SampleDesc.Count = 1;
	renderTargetDesc.SampleDesc.Quality = 0;
	renderTargetDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	renderTargetDesc.Flags= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_CLEAR_VALUE clearvalue;
	clearvalue .Format= resourceDesc.Format;
	
	clearvalue.Color[0] =resourceDesc.ClearColor[0];
	clearvalue.Color[1] = resourceDesc.ClearColor[1];
	clearvalue.Color[2] = resourceDesc.ClearColor[2];
	clearvalue.Color[3] = resourceDesc.ClearColor[3];
	
	return CreateTexture(gDevice, resourceDesc.ResourceState, &renderTargetDesc, &clearvalue);

}
 std::shared_ptr< GTexture> GTexture::CreateDepthStencilTexture(GraphicsDevice* GDevice, ResourceDescription resDesc)
{
	 D3D12_RESOURCE_DESC depthStencilDesc = {};
	 depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	 depthStencilDesc.Alignment = 0;
	 depthStencilDesc.Width = resDesc.Width;
	 depthStencilDesc.Height = resDesc.Height;

	 depthStencilDesc.DepthOrArraySize = 1;
	 depthStencilDesc.MipLevels = 1;

	 depthStencilDesc.Format = resDesc.Format;

	 depthStencilDesc.SampleDesc.Count = 1;
	 depthStencilDesc.SampleDesc.Quality = 0;
	 depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	 depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	 D3D12_CLEAR_VALUE optClear;
	 optClear.Format = GDevice->DepthStencilFormat;
	 optClear.DepthStencil.Depth = 1.0f;
	 optClear.DepthStencil.Stencil = 0;

	 return CreateTexture(GDevice, resDesc.ResourceState, &depthStencilDesc,&optClear);
}
 std::shared_ptr<GTexture> GTexture::CreateTexture(
	 GraphicsDevice* GDevice, 
	 D3D12_RESOURCE_STATES ResourceState,
	 D3D12_RESOURCE_DESC* d12Desc,
	 D3D12_CLEAR_VALUE* d3dClearValue)
 {

	 ComPtr<ID3D12Resource> resource;
	 ThrowGraphicsfFailed(GDevice->mD3dDevice->CreateCommittedResource
	 (
		 &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		 D3D12_HEAP_FLAG_NONE,
		 d12Desc,
		 ResourceState,
		 d3dClearValue,
		 IID_PPV_ARGS(resource.GetAddressOf())
	 ));
	 
	 std::shared_ptr<GTexture> tex=std::make_shared<GTexture>(resource);
	 return tex;
 }


 std::shared_ptr<GTexture> GTexture::CreateTextureFromFile(GraphicsDevice* gDevice, std::string& filename)
 {
	 
	 std::shared_ptr<GTexture> tex = std::make_shared<GTexture>();
	 gDevice->FlushCommandQueue();
	gDevice->ResetDirectCommandAlloc();
	gDevice->ResetDirectCommandList();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	auto wFilename = converter.from_bytes(filename);
	ComPtr<ID3D12Resource> upload;
	DirectX::CreateDDSTextureFromFile12(
		gDevice->mD3dDevice.Get(),
		gDevice->mDirectCommandList.Get(),
		wFilename.c_str(),
		tex->Resource,
		upload
	);

	gDevice->ExctureCommands();
	gDevice->FlushCommandQueue();
	return tex;
 }

 std::shared_ptr<GTexture> GTexture::CreateTextureFromFile(GraphicsDevice* gDevice, std::wstring& filename)
 {

	 std::shared_ptr<GTexture> tex = std::make_shared<GTexture>();
	 gDevice->FlushCommandQueue();
	 gDevice->ResetDirectCommandAlloc();
	 gDevice->ResetDirectCommandList();
	 
	 ComPtr<ID3D12Resource> upload;
	 DirectX::CreateDDSTextureFromFile12(
		 gDevice->mD3dDevice.Get(),
		 gDevice->mDirectCommandList.Get(),
		 filename.c_str(),
		 tex->Resource,
		 upload
	 );

	 gDevice->ExctureCommands();
	 gDevice->FlushCommandQueue();
	 return tex;
 }
