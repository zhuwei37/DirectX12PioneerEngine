#pragma once
#include"IComponent.h"
#include"DirectXMath.h"
#include<vector>
class Transform:public IComponent
{
public:
	 void Awake()override;
	 void Update()override;
	 const char* GetName()const override;


	 Transform();
	 void UpdateMatrix();
	 void SetPosition(float x, float y, float z);
	 void SetScale(float x, float y, float z);
	 void SetRotate(float x, float y, float z);
	 DirectX::XMFLOAT3& GetPosition();
	 DirectX::XMFLOAT3& GetRotate();
	 DirectX::XMFLOAT3& GetScale();
	 void SetParent(Transform* parent);
	 void AddTransformChildren(Transform* child);
	 void RemoveTransformChildren(Transform* child);
	 DirectX::XMFLOAT4X4 GetWorldMatrix();
private:
	//父节点
	Transform* mParent=nullptr;
	//子节点列表
	std::vector<Transform*> mChilds;
	//位置
	DirectX::XMFLOAT3 mPosition;
	//旋转
	DirectX::XMFLOAT3 mRotate;
	//缩放
	DirectX::XMFLOAT3 mScale;
	//World
	DirectX::XMFLOAT4X4 mWorldMatrix;
	//Local
	DirectX::XMFLOAT4X4 mLocalMatrix;

};