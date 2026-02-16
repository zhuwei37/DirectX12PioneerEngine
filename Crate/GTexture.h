#pragma once
#include<string>
#include"GraphicsDevice.h"
#include"UploadData.h"
struct ResourceDescription
{
	UINT Width;
	UINT Height;
	DXGI_FORMAT Format;
	D3D12_RESOURCE_STATES ResourceState;
	float ClearColor[4];
	
};
class GTexture :public GraphicsResource
{
public:
	
	static std::shared_ptr<GTexture> CreateTexture(GraphicsDevice* GDevice, D3D12_RESOURCE_STATES ResourceState,D3D12_RESOURCE_DESC*, D3D12_CLEAR_VALUE* d3dClearValue = nullptr);
	//模板后台缓冲区
	static std::shared_ptr<GTexture> CreateDepthStencilTexture(GraphicsDevice*, ResourceDescription);
	static std::shared_ptr<GTexture> CreateRenderTargetTexture(GraphicsDevice*, ResourceDescription);

	static std::shared_ptr<GTexture> CreateTextureFromFile(GraphicsDevice* GDevice, std::string&);
	static std::shared_ptr<GTexture> CreateTextureFromFile(GraphicsDevice* GDevice, std::wstring&);
	UINT GetWidth()const;
	UINT GetHeight()const;
	/*UINT Width;
	UINT Height;*/
	GTexture(ComPtr<ID3D12Resource>);
	GTexture() {}
protected:
	GTexture(GraphicsDevice* GDevice, D3D12_RESOURCE_DESC) = delete;
	
	
};