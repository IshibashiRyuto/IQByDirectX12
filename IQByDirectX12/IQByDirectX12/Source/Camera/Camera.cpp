#include "Camera.h"

/* íËêîíËã` */
namespace
{
	Math::Vector3 Z_AXIS(0.0f, 0.0f, 1.0f); //! Zé≤ê≥ÇÃå¸Ç´
}
Camera::Camera(const Math::Vector3& pos,
	const Math::Vector3& direction,
	ProjectionType projType,
	const ProjectionParam& projParam)
	: mPosition(pos)
	, mDirection(Math::CreateIdentQuaternion())
	, mUpper(0.0f, 1.0f, 0.0f)
{
	CreateProjMat[ProjectionType::Perspective] = CreatePerspectiveMatrix;
	CreateProjMat[ProjectionType::Orthographic] = CreateOrthographicMatrix;

	SetDirection(direction);

	mProjMat = CreateProjMat[projType](projParam);
	_UpdateViewMat();
}


Camera::~Camera()
{
}

std::shared_ptr<Camera> Camera::Create(const Math::Vector3 & pos, const Math::Vector3 & direction, ProjectionType projType, const ProjectionParam & projParam)
{
	return std::shared_ptr<Camera>(new Camera(pos, direction, projType, projParam));
}

void Camera::SetTargetPos(const Math::Vector3 & targetPos)
{
	SetDirection(targetPos - mPosition);
	mIsUpdate = true;
}

void Camera::SetPos(const Math::Vector3 & pos)
{
	mPosition = pos;
	mIsUpdate = true;
}

void Camera::SetDirection(const Math::Vector3 & direction)
{
	mDirection = Math::CreateRotVecToVec(Z_AXIS, direction.Normalized(), mUpper);
	mIsUpdate = true;
}

void Camera::SetDirection(const Math::Quaternion & direction)
{
	mDirection = direction;
	mIsUpdate = true;
}

void Camera::SetProjectionType(ProjectionType projType, const ProjectionParam & projParam)
{
	mProjMat = CreateProjMat[projType](projParam);
	mIsUpdate = true;
}

void Camera::Rotate(const Math::Quaternion & rotation)
{
	mDirection *= rotation;
	mIsUpdate = true;
}

void Camera::RotateWithUpper(const Math::Quaternion & rotation)
{
	mDirection *= rotation;
	mUpper = (rotation * Math::Quaternion(mUpper) * Math::CreateConjugateQuaternion(rotation)).v;
	mIsUpdate = true;
}

void Camera::Move(const Math::Vector3 & movement)
{
	mPosition += movement;
	mIsUpdate = true;
}

void Camera::UpdateMatrix()
{
	if (!mIsUpdate)
	{
		return;
	}
	_UpdateViewMat();
	mIsUpdate = false;
}

const Math::Matrix4x4 & Camera::GetViewMatrix() const
{
	return mViewMat;
}

const Math::Matrix4x4 & Camera::GetProjMatrix() const
{
	return mProjMat;
}

Math::Matrix4x4 Camera::CreatePerspectiveMatrix(const ProjectionParam & projParam)
{
	return Math::CreatePerspectiveMatrix(projParam.width/ projParam.height, projParam.nearZ, projParam.farZ, projParam.fov);
}

Math::Matrix4x4 Camera::CreateOrthographicMatrix(const ProjectionParam & projParam)
{
	return Math::CreateOrthoGraphicMatrix(projParam.width, projParam.height, projParam.nearZ, projParam.farZ);
}

void Camera::_UpdateViewMat()
{
	mViewMat = Math::CreateLookAtMatrix(mPosition, mDirection, mUpper);
}
