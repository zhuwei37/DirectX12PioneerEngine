#include"MeshImport.h"
shared_ptr<Mesh> MeshImport::LoadMesh(string filename)
{
	Assimp::Importer meshimport;
	/*auto p = import.ReadFile("E:\\BaiduNetdiskDownload\\DxRun\\assimp-6.0.2\\test\\models\\FBX\\box.fbx", aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded); */
	auto scene=meshimport.ReadFile(filename, aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (scene->HasMeshes())
	{
		shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		auto aiMesh=scene->mMeshes[0];


		mesh->mVertices.resize(aiMesh->mNumVertices);
		mesh->mIndices.resize(aiMesh->mFaces->mNumIndices);
		for (int i = 0; i < aiMesh->mNumVertices; i++)
		{
			GVertex v;
			v.Position = { aiMesh->mVertices[i].x,aiMesh->mVertices[i].y,aiMesh->mVertices[i].z };
			v.Normal = { aiMesh->mNormals[i].x,aiMesh->mNormals[i].y,aiMesh->mNormals[i].z };
			v.TexUV1 = { aiMesh->mTextureCoords[0][i].x,
				aiMesh->mTextureCoords[0][i].y,
			};
			mesh->mVertices[i] = v;
		}
		for (int i = 0; i < aiMesh->mFaces->mNumIndices; i++)
		{
			mesh->mIndices[i] = aiMesh->mFaces->mIndices[i];
		}
		mesh->mIndexBufferSize = mesh->mIndices.size() * sizeof(UINT);
		mesh->mVertexBufferSize = mesh->mVertices.size() * sizeof(GVertex);
		SubMeshDescriptor descriptor;
		descriptor.IndexCount = aiMesh->mFaces->mNumIndices;
		descriptor.IndexBaseStart = 0;
		descriptor.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		descriptor.VertexBaseStart = 0;
		mesh->SubMeshes.push_back(descriptor);
		return mesh;
	}
	return nullptr;
	
}