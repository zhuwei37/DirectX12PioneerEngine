#pragma once
#include"Mesh.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include <assimp/postprocess.h>
#include<string>
using namespace std;
class MeshImport
{
public:
	MeshImport() 
	{
	}
	shared_ptr<Mesh> LoadMesh(string filename);
};