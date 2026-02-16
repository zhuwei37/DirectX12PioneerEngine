#include "MeshFilter.h"


MeshFilter::MeshFilter()
{

}
void MeshFilter::Awake()
{
}

void MeshFilter::Update()
{
}

MeshResource* MeshFilter::GetMeshResource()
{
    return mMeshResource.get();
}
void MeshFilter::SetMeshResource(shared_ptr<MeshResource> mesh)
{
    this->mMeshResource = mesh;
}




const char* MeshFilter::GetName() const
{
    return "MeshFilter";
}
