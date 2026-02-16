#include"Mesh.h"
Mesh::Mesh(GeometryGenerator::MeshData& data,GraphicsDevice* gDevice)
{
	init(data, gDevice);
}
void Mesh::init(GeometryGenerator::MeshData& data, GraphicsDevice* gDevice)
{
	this->mIndices = data.Indices32;
	this->mVertices.resize(data.Vertices.size());
	for (int i = 0; i < data.Vertices.size(); i++)
	{
		this->mVertices[i].Position = data.Vertices[i].Position;
		this->mVertices[i].Normal = data.Vertices[i].Normal;
		this->mVertices[i].TexUV1 = data.Vertices[i].TexC;
		this->mVertices[i].TangentU = data.Vertices[i].TangentU;
	}
	this->mVertexBufferSize = sizeof(GVertex) * this->mVertices.size();
	this->mIndexBufferSize = sizeof(UINT) * this->mIndices.size();

	this->BindingGraphicsResource(gDevice);
	SubMeshDescriptor sub;
	sub.IndexBaseStart = 0;;
	sub.IndexCount = this->mIndices.size();
	sub.VertexBaseStart = 0;
	sub.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	this->SubMeshes.push_back(sub);
}