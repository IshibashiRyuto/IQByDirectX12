#include <cassert>
#include <cmath>
#include <utility>
#include "Math.h"

using namespace Math;

Vector3::Vector3()
	: Vector3(0.0f,0.0f,0.0f)
{
}

Vector3::Vector3(float mx, float my, float mz)
	: x(mx)
	, y(my)
	, z(mz)
{
}

Vector3::Vector3(const Vector3& other)
	: Vector3(other.x, other.y, other.z)
{
}

Math::Vector3::Vector3(Vector3 && other)
{
	*this = std::move(other);
}

Vector3::Vector3(const Vector4& other)
	: Vector3(other.x, other.y, other.z)
{
}

Math::Vector3::Vector3(const Quaternion & other)
{
	Vector3(other.v);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator+() const
{
	return Vector3(x, y, z);
}

Vector3& Vector3::operator=(const Vector3& value)
{
	this->x = value.x;
	this->y = value.y;
	this->z = value.z;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& value)
{
	this->x += value.x;
	this->y += value.y;
	this->z += value.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& value)
{
	this->x -= value.x;
	this->y -= value.y;
	this->z -= value.z;
	return *this;
}

Vector3& Vector3::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	return *this;
}

Vector3& Vector3::operator/=(float scale)
{
	assert(!IsZero(scale));

	this->x /= scale;
	this->y /= scale;
	this->z /= scale;
	return *this;
}

float Math::Vector3::Length() const
{
	return sqrt(LengthSquare());
}

float Math::Vector3::LengthSquare() const
{
	return x*x + y*y + z*z;
}

bool Math::operator== (const Vector3& t1, const Vector3& t2)
{
	return (IsEqual(t1.x,t2.x) && IsEqual(t1.y,t2.y));
}

bool Math::operator!=(const Vector3& t1, const Vector3& t2)
{
	return !(t1 == t2);
}

Vector3 Math::operator+(const Vector3& t1, const Vector3& t2)
{
	return Vector3(t1) += t2;
}

Vector3 Math::operator-(const Vector3& t1, const Vector3& t2)
{
	return Vector3(t1) -= t2;
}

Vector3 Math::operator*(const Vector3& value, float scale)
{
	return Vector3(value) *= scale;
}

Vector3 Math::operator*(float scale, const Vector3& value)
{
	return Vector3(value) *= scale;
}

Vector3 Math::operator/(const Vector3& value, float scale)
{
	return Vector3(value) /= scale;
}

float Math::Dot(const Vector3 & v1, const Vector3 & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Math::Cross(const Vector3 & v1, const Vector3 & v2)
{
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

Vector3 Math::Normalize(const Vector3 & vec)
{
	float norm = vec.Length();
	return Vector3(vec)/=norm;
}

float Math::CalcAngleVecToVec(const Vector3 & v1, const Vector3 & v2)
{
	return acos( Dot(v1,v2) / (v1.Length() * v2.Length()) );
}
