#include <cassert>
#include <utility>
#include "Math.h"

using namespace Math;

Vector4::Vector4()
	: Vector4(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Vector4::Vector4(float mx, float my, float mz, float mw)
	: x(mx)
	, y(my)
	, z(mz)
	, w(mw)
{
}

Vector4::Vector4(const Vector3& other)
	: Vector4(other.x, other.y, other.z, 1.0f)
{
}

Vector4::Vector4(const Vector4& other)
	: Vector4(other.x, other.y, other.z, other.w)
{
}

Math::Vector4::Vector4(Vector4 && other)
{
	*this = std::move(other);
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator+() const
{
	return Vector4(x, y, z, w);
}

Vector4& Vector4::operator=(const Vector4& value)
{
	this->x = value.x;
	this->y = value.y;
	this->z = value.z;
	this->w = value.w;
	return *this;
}

Vector4& Vector4::operator+=(const Vector4& value)
{
	this->x += value.x;
	this->y += value.y;
	this->z += value.z;
	this->w += value.w;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& value)
{
	this->x -= value.x;
	this->y -= value.y;
	this->z -= value.z;
	this->w -= value.w;
	return *this;
}

Vector4& Vector4::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	this->w *= scale;
	return *this;
}

Vector4& Vector4::operator/=(float scale)
{
	assert(!IsZero(scale));

	this->x /= scale;
	this->y /= scale;
	this->z /= scale;
	this->w /= scale;
	return *this;
}

bool Math::operator==(const Vector4& t1, const Vector4& t2)
{
	return (IsEqual(t1.x,t2.x) && IsEqual(t1.y,t2.y));
}

bool Math::operator!=(const Vector4& t1, const Vector4& t2)
{
	return !(t1 == t2);
}

Vector4 Math::operator+(const Vector4& t1, const Vector4& t2)
{
	return Vector4(t1) += t2;
}

Vector4 Math::operator-(const Vector4& t1, const Vector4& t2)
{
	return Vector4(t1) -= t2;
}

Vector4 Math::operator*(const Vector4& value, float scale)
{
	return Vector4(value) *= scale;
}

Vector4 Math::operator*(float scale, const Vector4& value)
{
	return Vector4(value) *= scale;
}

Vector4 Math::operator/(const Vector4& value, float scale)
{
	return Vector4(value) /= scale;
}