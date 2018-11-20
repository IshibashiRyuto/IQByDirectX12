#include <windows.h>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <utility>
#include "Math.h"

using namespace Math;

Matrix3x3::Matrix3x3()
	: _11(0.0f), _12(0.0f), _13(0.0f)
	, _21(0.0f), _22(0.0f), _23(0.0f)
	, _31(0.0f), _32(0.0f), _33(0.0f)
{
}

Math::Matrix3x3::Matrix3x3(const Matrix3x3 & mat)
	: _11(mat._11), _12(mat._12), _13(mat._13)
	, _21(mat._21), _22(mat._22), _23(mat._23)
	, _31(mat._31), _32(mat._32), _33(mat._33)
{
}

Math::Matrix3x3::Matrix3x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
	: _11(m11), _12(m22), _13(m13)
	, _21(m21), _22(m22), _23(m23)
	, _31(m31), _32(m32), _33(m33)
{
}

float & Math::Matrix3x3::operator()(unsigned int row, unsigned int col)
{
	row = min(max(row, 0), 3);
	col = min(max(col, 0), 3);
	return matrix[row][col];
}

const float & Math::Matrix3x3::operator()(unsigned int row, unsigned int col) const
{
	row = min(max(row, 0), 3);
	col = min(max(col, 0), 3);
	return matrix[row][col];
}

Matrix3x3 Math::Matrix3x3::operator+() const
{
	return Matrix3x3(*this);
}

Matrix3x3 Math::Matrix3x3::operator-() const
{
	return Matrix3x3(-_11, -_12, -_13,-_21, -_22, -_23, -_31, -_32, -_33);
}

Matrix3x3 & Math::Matrix3x3::operator=(const Matrix3x3 & value)
{
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] = value.matrix[row][col];
		}
	}
	return *this;
}

Matrix3x3 & Math::Matrix3x3::operator+=(const Matrix3x3 & value)
{
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] += value.matrix[row][col];
		}
	}
	return *this;
}

Matrix3x3 & Math::Matrix3x3::operator-=(const Matrix3x3 & value)
{
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] -= value.matrix[row][col];
		}
	}
	return *this;
}

Matrix3x3 & Math::Matrix3x3::operator*=(const Matrix3x3 & value)
{
	Matrix3x3 tmp;
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			tmp.matrix[row][col] = 0.0f;
			for (int i = 0; i < 3; ++i)
			{
				tmp.matrix[row][col] += this->matrix[row][i] * value.matrix[i][col];
			}
		}
	}
	*this = tmp;

	return *this;
}

Matrix3x3 & Math::Matrix3x3::operator*=(float scale)
{
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] *= scale;
		}
	}
	return *this;
}

Matrix3x3 & Math::Matrix3x3::operator/=(float scale)
{
	assert(!IsZero(scale));
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			this->matrix[row][col] /= scale;
		}
	}
	return *this;
}

float Math::Matrix3x3::Determinant() const
{
	return 
		(
			  _11 * _22 * _33
			+ _12 * _23 * _31
			+ _13 * _21 * _32
			- _11 * _23 * _32
			- _12 * _21 * _33
			- _13 * _22 * _31
		);
}


bool Math::operator==(const Matrix3x3 & mat1, const Matrix3x3 & mat2)
{
	for (int row = 0; row < 3; ++row)
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

bool Math::operator!=(const Matrix3x3 & mat1, const Matrix3x3 & mat2)
{
	return !(mat1 == mat2);
}

Matrix3x3 Math::operator+(const Matrix3x3 & mat1, const Matrix3x3 & mat2)
{
	return Matrix3x3(mat1) += mat2;
}

Matrix3x3 Math::operator-(const Matrix3x3 & mat1, const Matrix3x3 & mat2)
{
	return Matrix3x3(mat1) -= mat2;
}

Matrix3x3 Math::operator*(const Matrix3x3 & mat1, const Matrix3x3 & mat2)
{
	return Matrix3x3(mat1) *= mat2;
}

Matrix3x3 Math::operator*(const Matrix3x3 & mat1, float scale)
{
	return Matrix3x3(mat1) *= scale;
}

Matrix3x3 Math::operator*(float scale, const Matrix3x3 & mat2)
{
	return Matrix3x3(mat2) * scale;
}

Vector2 Math::operator*(const Vector2 & vec, const Matrix3x3 & mat)
{
	auto tmp = Vector3(vec.x, vec.y, 1.0f) * mat;
	return Vector2(tmp.x, tmp.y);
}

Vector3 Math::operator*(const Vector3 & vec, const Matrix3x3 & mat)
{
	Vector3 retVec;
	retVec.x = mat._11 * vec.x + mat._21 * vec.y + mat._31 * vec.z;
	retVec.y = mat._12 * vec.x + mat._22 * vec.y + mat._32 * vec.z;
	retVec.z = mat._13 * vec.x + mat._23 * vec.y + mat._33 * vec.z;
	return retVec;
}

Matrix3x3 Math::operator/(const Matrix3x3 & mat1, float scale)
{
	return Matrix3x3(mat1) /= scale;
}

Vector2 Math::GetMatrixScale(const Matrix3x3 & mat)
{
	return Vector2
	(
		Vector2(mat(0,0), mat(0,1)).Length(),
		Vector2(mat(1, 0), mat(1, 1)).Length()
	);
}

Vector2 Math::GetMatrixTranslate(const Matrix3x3 & mat)
{
	return Vector2(mat(2,0), mat(2,1));
}

float Math::GetMatrixRotation(const Matrix3x3 & mat)
{
	auto rotVec = Vector2(1, 0) * mat;
	return atan2(rotVec.y, rotVec.x);
}

Matrix3x3 Math::GetTransposeMatrix(const Matrix3x3 & mat)
{
	return Matrix3x3
	(
		mat(0, 0), mat(1, 0), mat(2, 0),
		mat(0, 1), mat(1, 1), mat(2, 1),
		mat(0, 2), mat(1, 2), mat(2, 2)
	);
}

Matrix3x3 Math::GetInvertMatrix(const Matrix3x3 & mat)
{
	float det = mat.Determinant();
	if (IsZero(det))
	{
		return CreateIdentMat3x3();
	}
	float m11 =  (mat(1, 1) * mat(2, 2) - mat(1, 2)*mat(2, 1));
	float m12 = -(mat(0, 1) * mat(2, 2) - mat(0, 2)*mat(2, 1));
	float m13 =  (mat(0, 1) * mat(1, 2) - mat(0, 2)*mat(1, 1));
	float m21 = -(mat(1, 0) * mat(2, 2) - mat(1, 2)*mat(2, 0));
	float m22 =  (mat(0, 0) * mat(2, 2) - mat(0, 2)*mat(2, 0));
	float m23 = -(mat(0, 0) * mat(1, 2) - mat(0, 2)*mat(1, 0));
	float m31 =  (mat(1, 0) * mat(2, 1) - mat(1, 1)*mat(2, 0));
	float m32 = -(mat(0, 0) * mat(2, 1) - mat(0, 1)*mat(2, 0));
	float m33 =  (mat(0, 0) * mat(1, 1) - mat(0, 1)*mat(1, 0));
	return Matrix3x3(m11,m12,m13,m21,m22,m23,m31,m32,33) /det;
}

Matrix3x3 Math::CreateIdentMat3x3()
{
	return Matrix3x3
	(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

Matrix3x3 Math::CreateScaleMatrix3x3(float scale)
{
	return Matrix3x3
	(
		scale, 0.0f, 0.0f,
		0.0f, scale, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

Matrix3x3 Math::CreateScaleMatrix3x3(float sx, float sy)
{
	return Matrix3x3
	(
		sx, 0.0f, 0.0f,
		0.0f, sy, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

Matrix3x3 Math::CreateScaleMatrix3x3(const Vector2 scale)
{
	return Matrix3x3
	(
		scale.x, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

Matrix3x3 Math::CreateTranslateMatrix3x3(float tx, float ty)
{
	auto tmp = CreateIdentMat3x3();
	tmp(2, 0) = tx;
	tmp(2, 1) = ty;
	return tmp;
}

Matrix3x3 Math::CreateTranslateMatrix3x3(const Vector2 & movement)
{
	auto tmp = CreateIdentMat3x3();
	tmp(2, 0) = movement.x;
	tmp(2, 1) = movement.y;
	return tmp;
}

Matrix3x3 Math::CreateRotMatrix3x3(float rad)
{
	return Matrix3x3
	(
		cos(rad), -sin(rad), 0.0f,
		sin(rad), cos(rad), 0.0f,
		0.0f, 0.0f, 1.0f
	);
}
