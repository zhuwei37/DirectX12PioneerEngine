#pragma once
#include"GTexture.h"
#include"ShaderResourceView.h"
class Texture
{
public:
	static Texture* GetDefualtTexture()
	{
		static Texture tex(true);
		return &tex;
	}
public:
	Texture();
	Texture(std::shared_ptr<GTexture>,std::shared_ptr<ShaderResourceView> srv);
	GTexture* GetTextureGraphicsResource();
	
	ShaderResourceView* GetShaderResourceView()
	{
		return gSrv.get();
	}
	
	void SetTextureGraphicsResource(std::shared_ptr<GTexture>);
private:
	std::shared_ptr<GTexture> gTex=nullptr;
	std::shared_ptr<ShaderResourceView> gSrv=nullptr;
	Texture(bool b)
	{
		D3D12_RESOURCE_DESC desc;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = 1;
		desc.Height = 1;

		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;

		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		auto tex=GTexture::CreateTexture(GraphicsDevice::Get(), D3D12_RESOURCE_STATE_COMMON, &desc);
		auto srv=ShaderResourceView::CreateShaderView(GraphicsDevice::Get(), tex.get());
		this->gSrv = srv;
		this->gTex = tex;
	}
};