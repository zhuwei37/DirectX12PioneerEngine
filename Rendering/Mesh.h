#pragma once
#include<DirectXMath.h>
#include"d3d12.h"
#include<vector>
#include<wrl.h>
#include"UploadData.h"
#include"GeometryGenerator.h"
using namespace Microsoft::WRL;
using namespace std;
typedef struct GVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexUV1;
	DirectX::XMFLOAT3 TangentU;
}GVertex;
typedef struct SubMeshDescriptor
{
public:
	UINT IndexCount;
	UINT VertexBaseStart;
	UINT IndexBaseStart;
	/// <summary>
	/// Í¼ÐÎÍØÆË
	/// </summary>
	D3D12_PRIMITIVE_TOPOLOGY Topology;
} SubMeshDescriptor;

class Mesh
{
public:
	Mesh() {}
	Mesh(GeometryGenerator::MeshData& data, GraphicsDevice* gDevice);

	void init(GeometryGenerator::MeshData& data, GraphicsDevice* gDevice);

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = mVertexGraphics->NavigateResource()->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexStride;
		vbv.SizeInBytes = mVertexBufferSize;
		return vbv;
	}
	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = mIndexGraphics->NavigateResource()->GetGPUVirtualAddress();
		ibv.SizeInBytes = mIndexBufferSize;
		ibv.Format = DXGI_FORMAT_R32_UINT;
		return ibv;
	}
	vector<UINT> mIndices;
	vector<GVertex> mVertices;

	vector<SubMeshDescriptor> SubMeshes;

	UINT VertexStride = sizeof(GVertex);
	UINT mVertexBufferSize;
	UINT mIndexBufferSize;
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
public :
	
	void BindingGraphicsResource(GraphicsDevice* gDevice)
	{
		 mVertexGraphics = GraphicsResource::CreateBuffer(gDevice, mVertexBufferSize,reinterpret_cast<BYTE*>(  mVertices.data()), D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_GENERIC_READ);
		 mIndexGraphics = GraphicsResource::CreateBuffer(gDevice, mIndexBufferSize, reinterpret_cast<BYTE*>(mIndices.data()), D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_GENERIC_READ);
	}
	~Mesh()
	{
		if (mVertexGraphics != nullptr)
		{
			delete mVertexGraphics;
		}
		if (mIndexGraphics != nullptr)
		{
			delete mIndexGraphics;
		}
	}
private:
	GraphicsResource* mVertexGraphics;
	GraphicsResource* mIndexGraphics;

};