#include <Windows.h>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <utility>
#include "Math.h"

using namespace Math;

Matrix4x3::Matrix4x3()
	: _11(0.f), _12(0.f), _13(0.f)
	, _21(0.f), _22(0.f), _23(0.f)
	, _31(0.f), _32(0.f), _33(0.f)
	, _41(0.f), _42(0.f), _43(0.f)
{
}

Math::Matrix4x3::Matrix4x3(const Matrix4x3 & mat)
	: _11(mat._11), _12(mat._12), _13(mat._13)
	, _21(mat._21), _22(mat._22), _23(mat._23)
	, _31(mat._31), _32(mat._32), _33(mat._33)
	, _41(mat._41), _42(mat._42), _43(mat._43)
{
}

Math::Matrix4x3::Matrix4x3(const Matrix4x4 & mat)
	: _11(mat._11), _12(mat._12), _13(mat._13)
	, _21(mat._21), _22(mat._22), _23(mat._23)
	, _31(mat._31), _32(mat._32), _33(mat._33)
	, _41(mat._41), _42(mat._42), _43(mat._43)
{
}

Math::Matrix4x3::Matrix4x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33, float m41, float m42, float m43)
	: _11(m11), _12(m12), _13(m13)
	, _21(m21), _22(m22), _23(m23)
	, _31(m31), _32(m32), _33(m33)
	, _41(m41), _42(m42), _43(m43)
{
}

float & Math::Matrix4x3::operator()(unsigned int row, unsigned int col)
{
	row = min(max(row, 0), 3);
	col = min(max(col, 0), 2);
	return matrix[row][col];
}

const float & Math::Matrix4x3::operator()(unsigned int row, unsigned int col) const
{
	row = min(max(row, 0), 3);
	col = min(max(col, 0), 3);
	return matrix[row][col];
}

Matrix4x3 Math::Matrix4x3::operator+() const
{
	return Matrix4x3(*this);
}

Matrix4x3 Math::Matrix4x3::operator-() const
{
	return Matrix4x3(-_11, -_12, -_13,
		-_21, -_22, -_23,
		-_31, -_32, -_33,
		-_41, -_42, -_43);
}

Matrix4x3 & Math::Matrix4x3::operator=(const Matrix4x3 & value)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] = value.matrix[row][col];
		}
	}
	return *this;
}

Matrix4x3 & Math::Matrix4x3::operator+=(const Matrix4x3 & value)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] += value.matrix[row][col];
		}
	}
	return *this;
}

Matrix4x3 & Math::Matrix4x3::operator-=(const Matrix4x3 & value)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] -= value.matrix[row][col];
		}
	}
	return *this;
}

Matrix4x3 & Math::Matrix4x3::operator*=(const Matrix4x3 & value)
{
	*this = Matrix4x3(Matrix4x4(*this) * Matrix4x4(value));
	return *this;
}

Matrix4x3 & Math::Matrix4x3::operator*=(float scale)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] *= scale;
		}
	}
	return *this;
}

Matrix4x3 & Math::Matrix4x3::operator/=(float scale)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] /= scale;
		}
	}
	return *this;
}

float Math::Matrix4x3::Determinant() const
{
	return
		(
			_11 * _22 * _33 * 1.0f
			+ _12 * _23 * _31 * 1.0f
			+ _13 * _21 * _32 * 1.0f
			- _11 * _23 * _32 * 1.0f
			- _12 * _21 * _33 * 1.0f
			- _13 * _22 * _31 * 1.0f
			);
}

bool Math::operator==(const Matrix4x3& mat1, const Matrix4x3& mat2)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			if (!IsEqual(mat1(row, col), mat2(row, col)))
			{
				return false;
			}
		}
	}
	return true;
}

bool Math::operator!=(const Matrix4x3 & mat1, const Matrix4x3 & mat2)
{
	return !(mat1 == mat2);
}

Matrix4x3 Math::operator+(const Matrix4x3 & mat1, const Matrix4x3 & mat2)
{
	return Matrix4x3(mat1) += mat2;
}

Matrix4x3 Math::operator-(const Matrix4x3 & mat1, const Matrix4x3 & mat2)
{
	return Matrix4x3(mat1) -= mat2;
}

Matrix4x3 Math::operator*(const Matrix4x3 & mat1, const Matrix4x3 & mat2)
{
	return Matrix4x3(mat1) *= mat2;
}

Matrix4x3 Math::operator*(const Matrix4x3 & mat, float scale)
{
	return Matrix4x3(mat) *= scale;
}

Matrix4x3 Math::operator*(float scale, const Matrix4x3 & mat)
{
	return Matrix4x3(mat) *= scale;
}

Vector3 Math::operator*(const Vector3 & vec, const Matrix4x3 & mat)
{
	return vec * Matrix4x4(mat);
}

Vector4 Math::operator*(const Vector4 & vec, const Matrix4x3 & mat)
{
	return vec * Matrix4x4(mat);
}

Matrix4x4 Math::operator/(const Matrix4x3 & mat, float scale)
{
	return Matrix4x4(mat) /= scale;
}
