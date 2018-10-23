#include <Windows.h>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <utility>
#include "Math.h"

using namespace Math;

Matrix4x4::Matrix4x4()
	: _11(0.0f), _12(0.0f), _13(0.0f), _14(0.0f)
	, _21(0.0f), _22(0.0f), _23(0.0f), _24(0.0f)
	, _31(0.0f), _32(0.0f), _33(0.0f), _34(0.0f)
	, _41(0.0f), _42(0.0f), _43(0.0f), _44(0.0f)
{
}

Matrix4x4::Matrix4x4(const Matrix4x4& mat)
	: _11(mat._11), _12(mat._12), _13(mat._13), _14(mat._14)
	, _21(mat._21), _22(mat._22), _23(mat._23), _24(mat._24)
	, _31(mat._31), _32(mat._32), _33(mat._33), _34(mat._34)
	, _41(mat._41), _42(mat._42), _43(mat._43), _44(mat._44)
{
}

Math::Matrix4x4::Matrix4x4(const Matrix4x3 & mat)
	: _11(mat._11), _12(mat._12), _13(mat._13), _14(0.f)
	, _21(mat._21), _22(mat._22), _23(mat._23), _24(0.f)
	, _31(mat._31), _32(mat._32), _33(mat._33), _34(0.f)
	, _41(mat._41), _42(mat._42), _43(mat._43), _44(1.f)
{
}

Math::Matrix4x4::Matrix4x4(Matrix4x4 && mat)
{
	*this = std::move(mat);
}

Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
	: _11(m11), _12(m12), _13(m13), _14(m14)
	, _21(m21), _22(m22), _23(m23), _24(m24)
	, _31(m31), _32(m32), _33(m33), _34(m34)
	, _41(m41), _42(m42), _43(m43), _44(m44)
{

}

float& Matrix4x4::operator()(unsigned int row, unsigned int col)
{
	row = min(max(row, 0), 3);
	col = min(max(col, 0), 3);
	return matrix[row][col];
}

const float& Matrix4x4::operator()(unsigned int row, unsigned int col) const
{
	row = min(max(row, 0), 3);
	col = min(max(col, 0), 3);
	return matrix[row][col];
}

Matrix4x4 Matrix4x4::operator+() const
{
	return Matrix4x4(*this);
}

Matrix4x4 Matrix4x4::operator-() const
{
	return Matrix4x4(-_11, -_12, -_13, -_14
		, -_21, -_22, -_23, -_24
		, -_31, -_32, -_33, -_34
		, -_41, -_42, -_43, -_44);
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& value)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			this->matrix[row][col] = value.matrix[row][col];
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& value)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			this->matrix[row][col] += value.matrix[row][col];
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& value)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			this->matrix[row][col] -= value.matrix[row][col];
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& value)
{
	Matrix4x4 tmp;

	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			tmp.matrix[row][col] = 0.0f;
			for (int i = 0; i < 4; ++i)
			{
				tmp.matrix[row][col] += this->matrix[row][i] * value.matrix[i][col];
			}

		}
	}
	*this = tmp;

	return *this;
}

Matrix4x4& Matrix4x4::operator*=(float scale)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			this->matrix[row][col] *= scale;
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator/=(float scale)
{
	assert(!IsZero(scale));
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			this->matrix[row][col] /= scale;
		}
	}
	return *this;
}

float Matrix4x4::Determinant() const
{
	return 
		(
			  _11 * _22 * _33 * _44 
			+ _11 * _23 * _34 * _42 
			+ _11 * _24 * _32 * _43
			+ _12 * _21 * _34 * _43
			+ _12 * _23 * _31 * _44
			+ _12 * _24 * _33 * _41
			+ _13 * _21 * _32 * _44
			+ _13 * _22 * _34 * _41
			+ _13 * _24 * _31 * _42
			+ _14 * _21 * _33 * _42
			+ _14 * _22 * _31 * _43
			+ _14 * _23 * _32 * _41
			- _11 * _22 * _34 * _43
			- _11 * _23 * _32 * _44
			- _11 * _24 * _33 * _42 
			- _12 * _21 * _33 * _44
			- _12 * _23 * _34 * _41
			- _12 * _24 * _31 * _43
			- _13 * _21 * _34 * _42
			- _13 * _22 * _31 * _44
			- _13 * _24 * _32 * _41
			- _14 * _21 * _32 * _43
			- _14 * _22 * _33 * _41
			- _14 * _23 * _31 * _42
		);
}

bool Math::operator==(const Matrix4x4& mat1, const Matrix4x4& mat2)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			if (!IsEqual(mat1(row,col), mat2(row,col)))
			{
				return false;
			}
		}
	}
	return true;
}

bool Math::operator!=(const Matrix4x4& mat1, const Matrix4x4& mat2)
{
	return !(mat1 == mat2);
}

Matrix4x4 Math::operator+(const Matrix4x4& mat1, const Matrix4x4& mat2)
{
	return Matrix4x4(mat1) += mat2;
}

Matrix4x4 Math::operator-(const Matrix4x4& mat1, const Matrix4x4& mat2)
{
	return Matrix4x4(mat1) -= mat2;
}

Matrix4x4 Math::operator*(const Matrix4x4& mat1, const Matrix4x4& mat2)
{
	return Matrix4x4(mat1) *= mat2;
}

Matrix4x4 Math::operator*(const Matrix4x4& mat, float scale)
{
	return Matrix4x4(mat) *= scale;
}

Matrix4x4 Math::operator*(float scale, const Matrix4x4& mat)
{
	return Matrix4x4(mat) *= scale;
}

Vector3 Math::operator*(const Vector3 & vec, const Matrix4x4 & mat)
{
	return Vector3(Vector4(vec)*mat);
}

Vector4 Math::operator*(const Vector4 & vec, const Matrix4x4 & mat)
{
	Vector4 retVec;
	retVec.x = mat._11 * vec.x + mat._21 * vec.y + mat._31 * vec.z + mat._41 * vec.w;
	retVec.y = mat._12 * vec.x + mat._22 * vec.y + mat._32 * vec.z + mat._42 * vec.w;
	retVec.z = mat._13 * vec.x + mat._23 * vec.y + mat._33 * vec.z + mat._43 * vec.w;
	retVec.w = mat._14 * vec.x + mat._24 * vec.y + mat._34 * vec.z + mat._44 * vec.w;
	return retVec;
}

Matrix4x4 Math::operator/(const Matrix4x4& mat, float scale)
{
	return Matrix4x4(mat) /= scale;
}

Vector3 Math::GetMatrixScale(const Matrix4x4& mat)
{
	return Vector3
	(
		Vector3(mat(0, 0), mat(0, 1), mat(0, 2)).Length(),
		Vector3(mat(1, 0), mat(1, 1), mat(1, 2)).Length(),
		Vector3(mat(2, 0), mat(2, 1), mat(2, 2)).Length()
	);
}

Vector3 Math::GetMatrixTranslate(const Matrix4x4& mat)
{
	return Vector3(mat(3, 0), mat(3, 1), mat(3, 2));
}

Quaternion Math::GetMatrixRotation(const Matrix4x4 & mat)
{
	auto scale = GetMatrixScale(mat);
	auto rotMatrix = mat;
	rotMatrix._11 /= scale.x;
	rotMatrix._22 /= scale.y;
	rotMatrix._33 /= scale.z;
	float x, y, z, w;
	x =  rotMatrix._11 - rotMatrix._22 - rotMatrix._33 + 1.0f;
	y = -rotMatrix._11 + rotMatrix._22 - rotMatrix._33 + 1.0f;
	z = -rotMatrix._11 - rotMatrix._22 + rotMatrix._33 + 1.0f;
	w =  rotMatrix._11 + rotMatrix._22 + rotMatrix._33 + 1.0f;
	float mult;
	float v;
	if (x >= y && x >= z && x >= w)
	{
		v = sqrtf(x) * 0.5f;
		mult = 0.25f / v;

		x = v;
		y = (rotMatrix._12 + rotMatrix._21) * mult;
		z = (rotMatrix._31 + rotMatrix._13) * mult;
		w = (rotMatrix._23 - rotMatrix._32) * mult;
	}
	else if (y >= x && y >= z && y >= w)
	{
		v = sqrtf(y) * 0.5f;
		mult = 0.25f / v;

		x = (rotMatrix._12 + rotMatrix._21) * mult;
		y = v;
		z = (rotMatrix._23 + rotMatrix._32) * mult;
		w = (rotMatrix._31 - rotMatrix._13) * mult;
	}
	else if (z >= x && z >= y && z >= w)
	{
		v = sqrtf(z) * 0.5f;
		mult = 0.25f / v;

		x = (rotMatrix._31 + rotMatrix._13) * mult;
		y = (rotMatrix._23 + rotMatrix._32) * mult;
		z = v;
		w = (rotMatrix._12 - rotMatrix._21) * mult;
	}
	else if (w >= x && w >= y && w >= z)
	{

		v = sqrtf(w) * 0.5f;
		mult = 0.25f / v;

		x = (rotMatrix._23 - rotMatrix._32) * mult;
		y = (rotMatrix._31 - rotMatrix._13) * mult;
		z = (rotMatrix._12 - rotMatrix._21) * mult;
		w = v;
	}


	return Quaternion(w, x, y, z)/Quaternion(w, x, y, z).Norm();
}

Matrix4x4 Math::GetTransposeMatrix(const Matrix4x4& mat)
{
	return Matrix4x4
	(
		mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0),
		mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1),
		mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2),
		mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3)
	);
}

Matrix4x4 Math::GetInvertMatrix(const Matrix4x4& mat)
{
	float det = mat.Determinant();
	if (IsZero(det))
	{
		return Math::CreateIdent();
	}

	float m11 =	(mat(1, 1) * mat(2, 2) * mat(3, 3)) + (mat(1, 2) * mat(2, 3) * mat(3, 1)) + (mat(1, 3) * mat(2, 1) * mat(3, 2))
		- (mat(1, 3) * mat(2, 2) * mat(3, 1)) - (mat(1, 2) * mat(2, 1) * mat(3, 3)) - (mat(1, 1) * mat(2, 3) * mat(3, 2));
	
	float m12 = -1 * ((mat(0, 1) * mat(2, 2) * mat(3, 3)) + (mat(0, 2) * mat(2, 3) * mat(3, 1)) + (mat(0, 3) * mat(2, 1) * mat(3, 2))
		- (mat(0, 3) * mat(2, 2) * mat(3, 1)) - (mat(0, 2) * mat(2, 1) * mat(3, 3)) - (mat(0, 1) * mat(2, 3) * mat(3, 2)));

	float m13 = ((mat(0, 1) * mat(1, 2) * mat(3, 3)) + (mat(0, 2) * mat(1, 3) * mat(3, 1)) + (mat(0, 3) * mat(1, 1) * mat(3, 2))
		- (mat(0, 3) * mat(1, 2) * mat(3, 1)) - (mat(0, 2) * mat(1, 1) * mat(3, 3)) - (mat(0, 1) * mat(1, 3) * mat(3, 2)));

	float m14 = -1 * ((mat(0, 1) * mat(1, 2) * mat(2, 3)) + (mat(0, 2) * mat(1, 3) * mat(2, 1)) + (mat(0, 3) * mat(1, 1) * mat(2, 2))
		- (mat(0, 3) * mat(1, 2) * mat(2, 1)) - (mat(0, 2) * mat(1, 1) * mat(2, 3)) - (mat(0, 1) * mat(1, 3) * mat(2, 2)));

	float m21 = -1 * (mat(1, 0) * mat(2, 2) * mat(3, 3)) + (mat(1, 2) * mat(2, 3) * mat(3, 0)) + (mat(1, 3) * mat(2, 0) * mat(3, 2))
		- (mat(1, 3) * mat(2, 2) * mat(3, 0)) - (mat(1, 2) * mat(2, 0) * mat(3, 3)) - (mat(1, 0) * mat(2, 3) * mat(3, 2));

	float m22 = (mat(0, 0) * mat(2, 2) * mat(3, 3)) + (mat(0, 2) * mat(2, 3) * mat(3, 0)) + (mat(0, 3) * mat(2, 0) * mat(3, 2))
		- (mat(0, 3) * mat(2, 2) * mat(3, 0)) - (mat(0, 2) * mat(2, 0) * mat(3, 3)) - (mat(0, 0) * mat(2, 3) * mat(3, 2));

	float m23 = -1 * (mat(0, 0) * mat(1, 2) * mat(3, 3)) + (mat(0, 2) * mat(1, 3) * mat(3, 0)) + (mat(0, 3) * mat(1, 0) * mat(3, 2))
		- (mat(0, 3) * mat(1, 2) * mat(3, 0)) - (mat(0, 2) * mat(1, 0) * mat(3, 3)) - (mat(0, 0) * mat(1, 3) * mat(3, 2));

	float m24 = (mat(0, 0) * mat(1, 2) * mat(2, 3)) + (mat(0, 2) * mat(1, 3) * mat(2, 0)) + (mat(0, 3) * mat(1, 0) * mat(2, 2))
		- (mat(0, 3) * mat(1, 2) * mat(2, 0)) - (mat(0, 2) * mat(1, 0) * mat(2, 3)) - (mat(0, 0) * mat(1, 3) * mat(2, 2));
	
	float m31 = (mat(1, 0) * mat(2, 1) * mat(3, 3)) + (mat(1, 1) * mat(2, 3) * mat(3, 0)) + (mat(1, 3) * mat(2, 0) * mat(3, 1))
		- (mat(1, 3) * mat(2, 1) * mat(3, 0)) - (mat(1, 1) * mat(2, 0) * mat(3, 3)) - (mat(1, 0) * mat(2, 3) * mat(3, 1));

	float m32 = -1 * (mat(0, 0) * mat(2, 1) * mat(3, 3)) + (mat(0, 1) * mat(2, 3) * mat(3, 0)) + (mat(0, 3) * mat(2, 0) * mat(3, 1))
		- (mat(0, 3) * mat(2, 1) * mat(3, 0)) - (mat(0, 1) * mat(2, 0) * mat(3, 3)) - (mat(0, 0) * mat(2, 3) * mat(3, 1));

	float m33 = (mat(0, 0) * mat(1, 1) * mat(3, 3)) + (mat(0, 1) * mat(1, 3) * mat(3, 0)) + (mat(0, 3) * mat(1, 0) * mat(3, 1))
		- (mat(0, 3) * mat(1, 1) * mat(3, 0)) - (mat(0, 1) * mat(1, 0) * mat(3, 3)) - (mat(0, 0) * mat(1, 3) * mat(3, 1));

	float m34 = -1 * (mat(0, 0) * mat(1, 1) * mat(2, 3)) + (mat(0, 1) * mat(1, 3) * mat(2, 0)) + (mat(0, 3) * mat(1, 0) * mat(2, 1))
		- (mat(0, 3) * mat(1, 1) * mat(2, 0)) - (mat(0, 1) * mat(1, 0) * mat(2, 3)) - (mat(0, 0) * mat(1, 3) * mat(2, 1));
	
	float m41 = -1 * (mat(1, 0) * mat(2, 1) * mat(3, 2)) + (mat(1, 1) * mat(2, 2) * mat(3, 0)) + (mat(1, 2) * mat(2, 0) * mat(3, 1))
		- (mat(1, 2) * mat(2, 1) * mat(3, 0)) - (mat(1, 1) * mat(2, 0) * mat(3, 2)) - (mat(1, 0) * mat(2, 2) * mat(3, 1));

	float m42 = (mat(0, 0) * mat(2, 1) * mat(3, 2)) + (mat(0, 1) * mat(2, 2) * mat(3, 0)) + (mat(0, 2) * mat(2, 0) * mat(3, 1))
		- (mat(0, 2) * mat(2, 1) * mat(3, 0)) - (mat(0, 1) * mat(2, 0) * mat(3, 2)) - (mat(0, 0) * mat(2, 2) * mat(3, 1));

	float m43 = -1 * (mat(0, 0) * mat(1, 1) * mat(3, 2)) + (mat(0, 1) * mat(1, 2) * mat(3, 0)) + (mat(0, 2) * mat(1, 0) * mat(3, 1))
		- (mat(0, 2) * mat(1, 1) * mat(3, 0)) - (mat(0, 1) * mat(1, 0) * mat(3, 2)) - (mat(0, 0) * mat(1, 2) * mat(3, 1));

	float m44 = (mat(0, 0) * mat(1, 1) * mat(2, 2)) + (mat(0, 1) * mat(1, 2) * mat(2, 0)) + (mat(0, 2) * mat(1, 0) * mat(2, 1))
		- (mat(0, 2) * mat(1, 1) * mat(2, 0)) - (mat(0, 1) * mat(1, 0) * mat(2, 2)) - (mat(0, 0) * mat(1, 2) * mat(2, 1));
	
	return Matrix4x4
	(
		m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44
	) *= (1.0f / det);
}

Matrix4x4 Math::CreateIdent()
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

Matrix4x4 Math::CreateScaleMatrix(float scale)
{
	return Matrix4x4
	(
		scale,0.0f,0.0f,0.0f,
		0.0f,scale,0.0f,0.0f,
		0.0f,0.0f,scale,0.0f,
		0.0f,0.0f,0.0f,1.0f
	);
}

Matrix4x4 Math::CreateScaleMatrix(float sx, float sy, float sz)
{
	return Matrix4x4
	(
		sx, 0.0f, 0.0f, 0.0f,
		0.0f, sy, 0.0f, 0.0f,
		0.0f, 0.0f, sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Math::CreateScaleMatrix(const Vector3& scale)
{
	return Matrix4x4
	(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Math::CreateTranslateMatrix(float tx, float ty, float tz)
{
	Matrix4x4 tmp = CreateIdent();
	tmp(3, 0) = tx;
	tmp(3, 1) = ty;
	tmp(3, 2) = tz;
	return tmp;
}

Matrix4x4 Math::CreateTranslateMatrix(const Vector3& movement)
{
	Matrix4x4 tmp = CreateIdent();
	tmp(3, 0) = movement.x;
	tmp(3, 1) = movement.y;
	tmp(3, 2) = movement.z;
	return tmp;
}

Matrix4x4 Math::CreateXRotMatrix(float rad)
{
	return Matrix4x4
	(
		1.0f,0.0f,0.0f,0.0f,
		0.0f, cos(rad),-sin(rad),0.0f,
		0.0f, sin(rad), cos(rad), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Math::CreateYRotMatrix(float rad)
{
	return Matrix4x4
	(
		cos(rad), 0.0f, sin(rad), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(rad), 0.0f, cos(rad), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Math::CreateZRotMatrix(float rad)
{
	return Matrix4x4
	(
		cos(rad), -sin(rad), 0.0f, 0.0f,
		sin(rad), cos(rad), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Math::CreateAxisRotMatrix(const Vector3& axis, float rad)
{
	auto nAxis = Normalize(axis);
	float tCos = cos(rad);
	float tSin = sin(rad);
	return Matrix4x4
	(
		nAxis.x * nAxis.x * (1 - tCos) + tCos, nAxis.x * nAxis.y * (1 - tCos) + nAxis.z * tSin, nAxis.x * nAxis.z * (1 - tCos) - nAxis.y * tSin, 0.0f,
		nAxis.x * nAxis.y * (1 - tCos) - nAxis.z * tSin, nAxis.y * nAxis.y * (1 - tCos) + tCos, nAxis.y * nAxis.z * (1 - tCos) + nAxis.x * tSin, 0.0f,
		nAxis.x * nAxis.z * (1 - tCos) + nAxis.y * tSin, nAxis.y * nAxis.z * (1 - tCos) - nAxis.x * tSin, nAxis.z * nAxis.z * (1 - tCos) + tCos , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Math::CreateLookAtMatrix(const Vector3 & eye, const Vector3 & target, const Vector3 & upper)
{
	auto zAxis = Normalize(target - eye);
	auto xAxis = Normalize(Cross(upper, zAxis));
	auto yAxis = Normalize(Cross(zAxis, xAxis));
	float xPos = Dot(xAxis, eye);
	float yPos = Dot(yAxis, eye);
	float zPos = Dot(zAxis, eye);
	return Matrix4x4
	(
		xAxis.x,		yAxis.x,		zAxis.x,	0.0f,
		xAxis.y,		yAxis.y,		zAxis.y,	0.0f,
		xAxis.z,		yAxis.z,		zAxis.z,	0.0f,
		-xPos,			-yPos,			-zPos,		1.0f
	);
}

Matrix4x4 Math::CreateLookAtMatrix(const Vector3 & eyePos, const Quaternion & direction, const Vector3 & upper)
{
	auto dirVec = (direction * Quaternion(Vector3(0.0f, 0.0f, 1.0f)) * CreateConjugateQuaternion(direction)).v;
	return Math::CreateLookAtMatrix(eyePos, eyePos + dirVec, upper);
}

Matrix4x4 Math::CreateLookAtMatrixFromCameraMatrix(const Matrix4x4 & cameraMatrix)
{
	auto retMatrix = GetTransposeMatrix(cameraMatrix);
	retMatrix(0, 3) = 0.0f;
	retMatrix(1, 3) = 0.0f;
	retMatrix(2, 3) = 0.0f;
	retMatrix(3, 0) = -cameraMatrix(3, 0);
	retMatrix(3, 1) = -cameraMatrix(3, 1);
	retMatrix(3, 2) = -cameraMatrix(3, 2);
	return retMatrix;
}

Matrix4x4 Math::CreatePerspectiveMatrix(float aspect, float nearZ, float farZ, float fov)
{
	auto tanFoV = tanf(fov/ 2.0f);
	return Matrix4x4
	(
		1.0f/aspect/tanFoV, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f/tanFoV, 0.0f, 0.0f,
		0.0f, 0.0f, farZ/(farZ-nearZ), 1.0f,
		0.0f, 0.0f, -(nearZ*farZ)/(farZ-nearZ), 0.0f
	);
}

Matrix4x4 Math::CreateOrthoGraphicMatrix(float width, float height, float nearZ, float farZ)
{
	return Matrix4x4
	(
		2.0f/width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f/height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f/(farZ - nearZ), 0.0f,
		0.0f, 0.0f, -nearZ/(farZ - nearZ), 1.0f
	);
}

