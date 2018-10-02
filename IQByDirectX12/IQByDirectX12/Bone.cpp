#include "Bone.h"




Bone::Bone(const Math::Vector3 & headPosition)
	: mOriginHeadPosition(headPosition)
	, mRotatedHeadPosition(headPosition)
	, mRotation(Math::CreateRotAxisQuaternion(Math::Vector3(0.0f,1.0f,0.0f), 0.0f))
	, mRotatedRotation(Math::CreateRotAxisQuaternion(Math::Vector3(0.0f, 1.0f, 0.0f), 0.0f))
	, mIsUseLimitAxis(false)
{
}

Bone::~Bone()
{
}

std::shared_ptr<Bone> Bone::Create(const Math::Vector3 & headPosition)
{
	return std::shared_ptr<Bone>(new Bone(headPosition));
}

void Bone::SetRotation(const Math::Quaternion & rotation)
{
	mRotation = rotation;
	if (mIsUseLimitAxis)
	{
		//mRotation.v = Math::Vector3();
	}
}

void Bone::SetRotation(const Math::Vector3 & rotation)
{
	SetRotation(Math::CreateRotXYZQuaternion(rotation));
}

void Bone::SetRotation(float rotX, float rotY, float rotZ)
{
	SetRotation(Math::Vector3(rotX, rotY, rotZ));
}

void Bone::SetLimitAxis(bool isUse, const Math::Vector3 & axis)
{
	mIsUseLimitAxis = isUse;
	mLimitAxis = axis;
	mLimitAxis = Math::Normalize(mLimitAxis);
}

void Bone::Rotate(const Math::Matrix4x4 & matrix)
{
	mRotatedHeadPosition = mOriginHeadPosition * matrix;
	mRotatedRotation = Math::GetMatrixRotation(matrix) * mRotation;
}

const Math::Quaternion & Bone::GetRotation() const
{
	return mRotation;
}

const Math::Matrix4x4 Bone::GetBoneMatrix() const
{
	return Math::CreateTranslateMatrix(-mOriginHeadPosition) * Math::GetMatrixFromQuat(mRotatedRotation) * Math::CreateTranslateMatrix(mRotatedHeadPosition);
}
