//#include"RenderManager.h"
//#include"Device.h"
//RenderManager* RenderManager::Get()
//{
//	static RenderManager manager;
//	return &manager;
//}
//void RenderManager::Init(HWND hwnd ,UINT width,UINT height)
//{
//	mApp = std::make_unique<CrateApp>();
//	mCamera = std::make_unique<CameraController>();
//	mCamera->SetPosition(0.0f, 2.0f, -15.0);
//	mCamera->SetLens(0.25f * MathHelper::Pi, static_cast<float>(width / height), 1.0f, 1000.0f);
//	mApp->mClientHeight = height;
//	mApp->mClientWidth = width;
//	mApp->mHandle = hwnd;
//	mApp->Initialize();
//		
//}
//RenderManager::RenderManager()
//{
//	
//	
//}
//
//void RenderManager::Update()
//{
//
//}
//void RenderManager::OnResize(int width, int height)
//{
//	if (width <= 0 || height <= 0)
//	{
//		return;
//	}
//	mWidth = width;
//	mHeight = height;
//	mApp->OnResize(width, height);
//	mCamera->Resize(width, height);
//	/*mSharedTexture->OnResize(width, height);*/
//}
//HANDLE RenderManager::GetSharedTextureHandle()
//{
//	
//	return 0;
//}
//
//
//
//
//void RenderManager::Draw()
//{
//	if (mApp != nullptr)
//	{
//		
//		mCamera->UpdateViewMatrix();
//		mApp->Update(mCamera->GetView(),mCamera->GetProj(),mCamera->GetPosition3f());
//		mApp->Draw();
//	}
//}
