#include "Bone.h"




Bone::Bone(const Math::Vector3 & headPosition)
	: mOriginHeadPosition(headPosition)
	, mRotatedHeadPosition(headPosition)
{
}

Bone::~Bone()
{
}

void Bone::SetRotation(const Math::Quaternion & rotation)
{
	mRotation = rotation;
}

void Bone::SetRotation(const Math::Vector3 & rotation)
{
	SetRotation(Math::CreateRotXYZQuaternion(rotation));
}

void Bone::SetRotation(float rotX, float rotY, float rotZ)
{
	SetRotation(Math::Vector3(rotX, rotY, rotZ));
}

void Bone::Rotate(const Math::Matrix4x4 & matrix)
{
	mRotatedHeadPosition = mOriginHeadPosition * matrix;
}

const Math::Quaternion & Bone::GetRotation() const
{
	return mRotation;
}

const Math::Matrix4x4 Bone::GetBoneMatrix() const
{
	return Math::CreateTranslateMatrix(-mRotatedHeadPosition) * Math::GetMatrixFromQuat(mRotation) * Math::CreateTranslateMatrix(mRotatedHeadPosition);
}
