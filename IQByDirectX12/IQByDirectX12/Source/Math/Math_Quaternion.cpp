#include <utility>
#include <algorithm>
#include <functional>
#include <cassert>
#include "Math.h"

/// @date
/// 2018/09/11	operator*=‚ÌƒoƒN‚ðC³
/// 2018/10/22	CreateRotVecToVec‚Ì‰ñ“]‚ª‰EŽèŒnŠî€‚É‚È‚Á‚Ä‚¢‚½‚½‚ßC³

using namespace Math;


Quaternion::Quaternion()
	: Quaternion(0.0f,0.0f,0.0f,0.0f)
{
}

Quaternion::Quaternion(float mw, float mx, float my, float mz)
	: w(mw)
	, x(mx)
	, y(my)
	, z(mz)
{
}

Quaternion::Quaternion(float mx, float my, float mz)
	: Quaternion(Math::Vector3( mx, my, mz) )
{
}

Math::Quaternion::Quaternion(const Math::Vector3 & vector)
	: w(0.0f)
	, v(vector)
{
}

Quaternion::Quaternion(const Quaternion& other)
	: Quaternion(other.w, other.x, other.y, other.z)
{
}

Quaternion::Quaternion(Quaternion&& other)
{
	*this = std::move(other);
}

Quaternion Quaternion::operator+() const
{
	return Quaternion(*this);
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-this->w, -this->x, -this->y, -this->z);
}

Quaternion& Quaternion::operator=(const Quaternion& quat)
{
	this->w = quat.w;
	this->x = quat.x;
	this->y = quat.y;
	this->z = quat.z;
	return *this;
}

Quaternion& Quaternion::operator+=(const Quaternion& quat)
{
	this->w += quat.w;
	this->x += quat.x;
	this->y += quat.y;
	this->z += quat.z;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& quat)
{
	this->w -= quat.w;
	this->x -= quat.x;
	this->y -= quat.y;
	this->z -= quat.z;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& quat)
{
	auto mw = this->w * quat.w - Dot(this->v, quat.v);
	auto mv = this->w * quat.v + quat.w * this->v - Cross(this->v, quat.v);
	this->w = mw;
	this->v = mv;
	return *this;
}

Quaternion & Math::Quaternion::operator*=(float scale)
{
	this->w *= scale;
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	return *this;
}

Quaternion & Math::Quaternion::operator/=(const Quaternion & quaternion)
{
	*this *= CreateInvertQuaternion(quaternion);
	return *this;
}

Quaternion & Math::Quaternion::operator/=(float scale)
{
	assert(!IsZero(scale));
	this->w /= scale;
	this->x /= scale;
	this->y /= scale;
	this->z /= scale;
	return *this;
}

float Math::Quaternion::Norm() const
{
	return sqrt(w*w + x*x + y*y + z*z);
}

float Math::Quaternion::NormSquare() const
{
	return (w * w + x * x + y * y + z * z);
}

bool Math::operator==(const Quaternion& q1, const Quaternion& q2)
{
	return (q1.w == q2.w) && (q1.x == q2.x) && (q1.y == q2.y) && (q1.z == q2.z);
}

bool Math::operator!=(const Quaternion& q1, const Quaternion& q2)
{
	return !(q1 == q2);
}

Quaternion Math::operator+(const Quaternion & q1, const Quaternion & q2)
{
	return Quaternion(q1) += q2;
}

Quaternion Math::operator-(const Quaternion & q1, const Quaternion & q2)
{
	return Quaternion(q1) -= q2;
}

Quaternion Math::operator*(const Quaternion & q1, const Quaternion & q2)
{
	return Quaternion(q1) *= q2;
}

Quaternion Math::operator*(const Quaternion & quat, float scale)
{
	return Quaternion(quat) *= scale;
}

Quaternion Math::operator*(float scale, const Quaternion & quat)
{
	return Quaternion(quat) *= scale;
}

Quaternion Math::operator/(const Quaternion & q1, const Quaternion & q2)
{
	return Quaternion(q1)/=q2;
}

Quaternion Math::operator/(const Quaternion & quat, float scale)
{
	return Quaternion(quat)/=scale;
}

Quaternion Math::CreateIdentQuaternion()
{
	return Quaternion(1.0f,0.0f,0.0f,0.0f);
}

Quaternion Math::CreateInvertQuaternion(const Quaternion & quat)
{
	return CreateConjugateQuaternion(quat) /= quat.NormSquare();
}

Quaternion Math::CreateConjugateQuaternion(const Quaternion & quat)
{
	return Quaternion(quat.w, -quat.x, -quat.y, -quat.z);
}

Quaternion Math::CreateRotAxisQuaternion(const Vector3 & axis, float angle)
{
	auto nAxis = Normalize(axis);
	float tSin = sin(angle / 2.0f);
	float tCos = cos(angle / 2.0f);
	return Quaternion(tCos, nAxis.x * tSin, nAxis.y * tSin, nAxis.z * tSin);
}

Quaternion Math::CreateRotXYZQuaternion(const Vector3 & rotAngle)
{
	Quaternion xAxisQuat = CreateRotAxisQuaternion(Vector3(1.0f, 0.0f, 0.0f), rotAngle.x);
	Quaternion yAxisQuat = CreateRotAxisQuaternion(Vector3(0.0f, 1.0f, 0.0f), rotAngle.y);
	Quaternion zAxisQuat = CreateRotAxisQuaternion(Vector3(0.0f, 0.0f, 1.0f), rotAngle.z);
	auto tmp = zAxisQuat * yAxisQuat;
	tmp = tmp * xAxisQuat;
	return zAxisQuat * yAxisQuat * xAxisQuat;
}

Quaternion Math::CreateRotVecToVec(const Vector3 & dstVec, const Vector3 & srcVec, const Vector3& up)
{
	auto rotAxis = Cross(dstVec, srcVec);
	auto rotAngle = CalcAngleVecToVec(dstVec, srcVec);
	return CreateRotAxisQuaternion(-rotAxis, rotAngle);
}

Quaternion Math::Lerp(const Quaternion & q1, const Quaternion q2, float t)
{
	t = Clamp(t, 0.0f, 1.0f);
	return Quaternion(q1 * (1.0f - t) + q2 * t);
}

Quaternion Math::Slerp(const Quaternion & q1, const Quaternion & q2, float t)
{
	t = Clamp(t, 0.0f, 1.0f);
	float angle = CalcAngleQuatToQuat(q1, q2);
	if (angle == 0.0f)
	{
		return q1;
	}
	return Quaternion(q1 * (sinf((1.0f - t) * angle) / sinf(angle)) + q2 * (sinf(t * angle) / sinf(angle)));
}

float Math::Dot(const Quaternion & q1, const Quaternion & q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

float Math::CalcAngleQuatToQuat(const Quaternion & q1, const Quaternion & q2)
{
	return acosf( Dot(q1, q2) / ( q1.Norm() * q2.Norm() ));
}

Matrix4x4 Math::GetMatrixFromQuat(const Quaternion & quat)
{
	return Matrix4x4
	(
		(1.0f - 2.0f * quat.y * quat.y - 2.0f * quat.z * quat.z),	(2.0f * quat.x*quat.y + 2.0f * quat.w*quat.z),			(2.0f * quat.x * quat.z - 2.0f * quat.w*quat.y), 0.0f,
		(2.0f * quat.x * quat.y - 2.0f * quat.w * quat.z),			(1.0f - 2.0f * quat.x*quat.x - 2.0f * quat.z*quat.z),	(2.0f * quat.y * quat.z + 2.0f * quat.w*quat.x), 0.0f,
		(2.0f * quat.x * quat.z + 2.0f * quat.w * quat.y),			(2.0f * quat.y * quat.z - 2.0f * quat.w*quat.x),		(1.0f - 2.0f * quat.x * quat.x - 2.0f * quat.y*quat.y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}
