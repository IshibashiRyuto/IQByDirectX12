#include <cassert>
#include <utility>
#include "Math.h"


using namespace Math;

Vector2::Vector2()
	: Vector2(0.0f, 0.0f)
{
}

Vector2::Vector2(float mx, float my)
	: x(mx)
	, y(my)
{
}

Vector2::Vector2(const Vector2& other)
	: Vector2(other.x, other.y)
{
}

Math::Vector2::Vector2(Vector2 && other)
{
	*this = std::move(other);
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator+() const
{
	return Vector2(x, y);
}

Vector2& Vector2::operator=(const Vector2& value)
{
	this->x = value.x;
	this->y = value.y;
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& value)
{
	this->x += value.x;
	this->y += value.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& value)
{
	this->x -= value.x;
	this->y -= value.y;
	return *this;
}

Vector2& Vector2::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
	return *this;
}

Vector2& Vector2::operator/=(float scale)
{
	assert(!IsZero(scale));

	this->x /= scale;
	this->y /= scale;
	return *this;
}

bool Math::operator==(const Vector2& t1, const Vector2& t2)
{
	return (IsEqual(t1.x,t2.x) && IsEqual(t1.y,t2.y));
}

bool Math::operator!=(const Vector2& t1, const Vector2& t2)
{
	return !(t1 == t2);
}

Vector2 Math::operator+(const Vector2& t1, const Vector2& t2)
{
	return Vector2(t1) += t2;
}

Vector2 Math::operator-(const Vector2& t1, const Vector2& t2)
{
	return Vector2(t1) -= t2;
}

Vector2 Math::operator*(const Vector2& value, float scale)
{
	return Vector2(value) *= scale;
}

Vector2 Math::operator*(float scale, const Vector2& value)
{
	return Vector2(value) *= scale;
}

Vector2 Math::operator/(const Vector2& value, float scale)
{
	return Vector2(value) /= scale;
}