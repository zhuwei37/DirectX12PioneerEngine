//#pragma once
//#include"d3dApp.h"
//#include "../../Common/UploadBuffer.h"
//#include "FrameResource.h"
//#include"SharedTexture.h"
//#include"Filter.h"
//#include "MathHelper.h"
//
//using Microsoft::WRL::ComPtr;
//using namespace DirectX;
//using namespace DirectX::PackedVector;
//
//
//// Lightweight structure stores parameters to draw a shape.  This will
//// vary from app-to-app.
//struct RenderItem
//{
//    RenderItem() = default;
//
//    // World matrix of the shape that describes the object's local space
//    // relative to the world space, which defines the position, orientation,
//    // and scale of the object in the world.
//    XMFLOAT4X4 World = MathHelper::Identity4x4();
//
//    XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();
//
//    // Dirty flag indicating the object data has changed and we need to update the constant buffer.
//    // Because we have an object cbuffer for each FrameResource, we have to apply the
//    // update to each FrameResource.  Thus, when we modify obect data we should set 
//    // NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
//    int NumFramesDirty = gNumFrameResources;
//
//    // Index into GPU constant buffer corresponding to the ObjectCB for this render item.
//    UINT ObjCBIndex = -1;
//    MeshGeometry* Geo = nullptr;
//
//    // Primitive topology.
//    D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//
//    // DrawIndexedInstanced parameters.
//    UINT IndexCount = 0;
//    UINT StartIndexLocation = 0;
//    int BaseVertexLocation = 0;
//};
//
//class CrateApp : public D3DApp
//{
//public:
//    CrateApp();
//    CrateApp(const CrateApp& rhs) = delete;
//    CrateApp& operator=(const CrateApp& rhs) = delete;
//    ~CrateApp();
// 
//    virtual bool Initialize()override;
//    virtual void OnResize(int width, int height)override;
//    void Draw();
//    void Update( XMMATRIX view, XMMATRIX proj, XMFLOAT3 pos);
//private:
//  
//
//   
//   
//    void UpdateObjectCBs();
//    void UpdateMainPassCB( XMMATRIX view, XMMATRIX proj, XMFLOAT3 pos);
//
//    void LoadTextures();;
//    void BuildShapeGeometry();
//    void BuildPSOs();
//    void BuildFrameResources();
//    void BuildRenderItems();
//    void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
//
//    std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
//
//private:
//
//    std::vector<std::unique_ptr<FrameResource>> mFrameResources;
//    FrameResource* mCurrFrameResource = nullptr;
//   // std::unique_ptr<MaterialContext>  mRenderContext;
//
//
//    //std::unique_ptr<ShaderResourceView> mSrv;
//
//    std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
//       // List of all the render items.
//    std::vector<std::unique_ptr<RenderItem>> mAllRitems;
//
//    // Render items divided by PSO.
//    std::vector<RenderItem*> mOpaqueRitems;
//    //std::shared_ptr<ShaderProperties> mMainShaderProperties;
//     PassConstants mMainPassCB;
//};