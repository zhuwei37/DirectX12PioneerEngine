#include"SharedTexture.h"


SharedTexture::SharedTexture(ID3D12Device* dev,int width,int height)
{
	device = dev;
	mWidth = width;
	mHeight = height;
}

void SharedTexture::CreateSharedTexture()
{
	 D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	 D3D12_RESOURCE_DESC texDesc;
	 ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	 texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;;
	 texDesc.Alignment = 0;
	 texDesc.Width = mWidth;
	 texDesc.Height = mHeight;
	 texDesc.DepthOrArraySize = 1;
	 texDesc.MipLevels = 1;
	 texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; 
	 texDesc.SampleDesc.Count = 1;
	 texDesc.SampleDesc.Quality = 1;
	 texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	 texDesc.Flags =
		  D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	 ThrowIfFailed( device->CreateCommittedResource(
		 &heapProps,
		 D3D12_HEAP_FLAG_SHARED,
		 &texDesc,
		 D3D12_RESOURCE_STATE_COMMON,
		 nullptr,
		 IID_PPV_ARGS(&mTexture)
	 ));
}
void SharedTexture::CreateSharedTextrueHandle()
{
	ThrowIfFailed(device->CreateSharedHandle(
		mTexture.Get(),
		nullptr,
		GENERIC_ALL,
		nullptr,
		&sharedHandle));
}
void SharedTexture::CopyTexture(ID3D12Resource* input, ID3D12GraphicsCommandList *mCmdList)
{
	mCmdList->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::
		Transition(mTexture.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST));
	mCmdList->CopyResource(mTexture.Get(), input);
	mCmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mTexture.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_COMMON));

}
HANDLE SharedTexture::GetSharedTextureHandle()
{
	if (mTexture != nullptr)
	{
		/*ComPtr<IDXGIResource> dxgiResource;
		mTexture->QueryInterface(IID_PPV_ARGS(&dxgiResource));
		HANDLE handle=nullptr;*/
		/*dxgiResource->GetSharedHandle(&handle);*/
		return sharedHandle;
	}
	return nullptr;
}
void SharedTexture::OnResize(int width,int height)
{
	mWidth = width;
	mHeight = height;
	CreateSharedTexture();
	CreateSharedTextrueHandle();
}