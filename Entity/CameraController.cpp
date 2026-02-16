#include "CameraController.h"
using namespace DirectX;
CameraController::CameraController()
{
	SetLens(0.25f * GMathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}
CameraController::CameraController(UINT width, UINT heigth)
{
	Resize(width, heigth);
}
CameraController::~CameraController()
{
}

XMVECTOR CameraController::GetPosition()const
{
	return XMLoadFloat3(&mPosition);
}
XMFLOAT3 CameraController::GetPosition3f()const
{
	return mPosition;
}
void CameraController::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
	mViewDirty = true;
}
void CameraController::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
	mViewDirty = true;
}
XMVECTOR CameraController::GetRight()const
{
	return XMLoadFloat3(&mRight);
}
XMFLOAT3 CameraController::GetRight3f()const
{
	return mRight;
}
XMVECTOR CameraController::GetUp()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 CameraController::GetUp3f()const
{
	return mUp;
}

XMVECTOR CameraController::GetLook()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 CameraController::GetLook3f()const
{
	return mLook;
}

float CameraController::GetNearZ()const
{
	return mNearZ;
}

float CameraController::GetFarZ()const
{
	return mFarZ;
}

float CameraController::GetAspect()const
{
	return mAspect;
}

float CameraController::GetFovY()const
{
	return mFovY;
}
float CameraController::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float CameraController::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float CameraController::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float CameraController::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float CameraController::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}
void CameraController::SetLens(float fovY, float aspect, float zn, float zf)
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);
}
void CameraController::Resize(UINT width, UINT height)
{
	SetLens(0.25f * GMathHelper::Pi,static_cast<float>( (float)width/(float)height), 1.0f, 1000.0f);
}
void CameraController::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);

	mViewDirty = true;
}

void CameraController::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);
	LookAt(P, T, U);
	mViewDirty = true;
}

XMMATRIX CameraController::GetView()const
{
	assert(!mViewDirty);
	return XMLoadFloat4x4(&mView);
}
XMMATRIX CameraController::GetProj()const
{
	return XMLoadFloat4x4(&mProj);
}
XMFLOAT4X4 CameraController::GetView4x4f()const
{
	assert(!mViewDirty);
	return mView;
}
XMFLOAT4X4 CameraController::GetProj4x4f()const
{
	return mProj;
}
void CameraController::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
	mViewDirty = true;
}


void CameraController::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
	mViewDirty = true;
}
void CameraController::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}
void CameraController::Roll(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mLook), angle);
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight),R));
	mViewDirty = true;
}
void CameraController::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}
void CameraController::UpdateViewMatrix()
{
	if (mViewDirty)
	{
		XMVECTOR R = XMLoadFloat3(&mRight);
		XMVECTOR U = XMLoadFloat3(&mUp);
		XMVECTOR L = XMLoadFloat3(&mLook);
		XMVECTOR P = XMLoadFloat3(&mPosition);

		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		R = XMVector3Cross(U, L);

		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		mView(0, 0) = mRight.x;
		mView(1, 0) = mRight.y;
		mView(2, 0) = mRight.z;
		mView(3, 0) = x;

		mView(0, 1) = mUp.x;
		mView(1, 1) = mUp.y;
		mView(2, 1) = mUp.z;
		mView(3, 1) = y;

		mView(0, 2) = mLook.x;
		mView(1, 2) = mLook.y;
		mView(2, 2) = mLook.z;
		mView(3, 2) = z;

		mView(0, 3) = 0.0f;
		mView(1, 3) = 0.0f;
		mView(2, 3) = 0.0f;
		mView(3, 3) = 1.0f;

		mViewDirty = false;
	}
}