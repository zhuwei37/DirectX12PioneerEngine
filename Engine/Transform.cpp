#include"Transform.h"
#include"GMathHelper.h"
Transform::Transform():
	mChilds(),
	mPosition(),
	mScale(1.0f,1.0f,1.0f),
	mRotate()
{
}
void Transform::SetPosition(float x, float y, float z)
{
	mPosition = { x,y,z };
}
void Transform::SetRotate(float x, float y, float z)
{
	mRotate = { x,y,z };
}
void Transform::SetScale(float x, float y, float z)
{
	mScale = { x,y,z };
}

DirectX::XMFLOAT3& Transform::GetPosition()
{
	return mPosition;
}
DirectX::XMFLOAT3& Transform::GetRotate()
{
	return mRotate;
}
DirectX::XMFLOAT3& Transform::GetScale()
{
	return mScale;
}
void Transform::Awake()
{
}
void Transform::Update()
{
}
const char* Transform::GetName()const
{
	return "Transform";
}

void Transform::SetParent(Transform* parent)
{
	if (parent != nullptr)
	{

	}
	this->mParent = parent;
}
void Transform::AddTransformChildren(Transform* child)
{
	mChilds.push_back(child);
}
void Transform::RemoveTransformChildren(Transform* child)
{
	auto removeItems = std::remove(mChilds.begin(), mChilds.end(), child);
	if (removeItems != mChilds.end())
	{
		mChilds.erase(removeItems);
	}
}
DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	return mWorldMatrix;
}
void Transform::UpdateMatrix()
{
	mLocalMatrix = GMathHelper::CreateSRTMatrix(mPosition, mRotate, mScale);
	DirectX::XMFLOAT4X4 parentMatrix;
	if (mParent != nullptr)
	{
		parentMatrix=mParent->GetWorldMatrix();
		
	}
	else
	{
		parentMatrix = GMathHelper::Identity4x4();
	}
	DirectX::XMMATRIX localMatrix = DirectX::XMLoadFloat4x4(&mLocalMatrix);
	DirectX::XMMATRIX parentWorldMatrix = DirectX::XMLoadFloat4x4(&parentMatrix);
	auto worldMatrix = DirectX::XMMatrixMultiply(localMatrix, parentWorldMatrix);
	DirectX::XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	for (auto& child : mChilds)
	{
		child->UpdateMatrix();
	}

}