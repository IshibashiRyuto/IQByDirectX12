/*
	@file Math.h
	@brief ���쐔�w�֘A���C�u����
	@author Ishibashi Ryuto
	@history 
	2018/07/19 ���ō쐬
	2018/08/26 �s��N���X�ɋ@�\��ǉ�
		�ELookAt�s��̐���
		�EPerspective�s��̐���
	2018/10/03	�f�[�^�ʍ팸�̂���4x3�s���ǉ�
*/
#pragma once
namespace Math
{
	// �\���̑O���錾
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix4x4;
	struct Matrix4x3;
	struct Quaternion;

	// �~������
	const float F_PI = 3.14159265f;

	// �~������(�{���x���������_�j
	const double D_PI = 3.141592653589793;

	/// @fn IsEqual
	/// ���������_�̓��l��r���s��
	/// @retval true: �l��������, false: �l���������Ȃ�
	bool IsEqual(float v1, float v2);
	bool IsEqual(double v1, double v2);
	bool IsEqual(float v1, double v2);
	bool IsEqual(double v1, float v2);

	/// @fn IsZero
	/// ���������_�̃[���l������s��
	/// @retval true: �l��0�ł���, false: �l��0�łȂ�
	bool IsZero(float value);
	bool IsZero(double value);

	/// @fn Clamp
	/// �l���w�肵���͈͓��Ɏ��܂�悤�ɂ��ĕԂ�
	/// @param[in] value: �ΏۂƂȂ�l
	/// @param[in] minValue: �ŏ��l
	/// @param[in] maxValue: �ő�l
	/// @retval �͈͓��ɃN�����v���ꂽ�l
	/// @note minValue > maxValue �̏ꍇ�Aassert����������
	float Clamp(float value, float minValue, float maxValue);
	double Clamp(double value, double minValue, double maxValue);

	// 2�����x�N�g��
	struct Vector2
	{
		float x;
		float y;

		Vector2();
		Vector2(float mx, float my);
		Vector2(const Vector2& other);
		Vector2(Vector2&& other);
		Vector2 operator-() const;
		Vector2 operator+() const;
		Vector2& operator=(const Vector2& value);
		Vector2& operator+=(const Vector2& value);
		Vector2& operator-=(const Vector2& value);
		Vector2& operator*=(float scale);
		Vector2& operator/=(float scale);
	};
	bool operator==(const Vector2& t1, const Vector2& t2);
	bool operator!=(const Vector2& t1, const Vector2& t2);
	Vector2 operator+(const Vector2& t1, const Vector2& t2);
	Vector2 operator-(const Vector2& t1, const Vector2& t2);
	Vector2 operator*(const Vector2& value, float scale);
	Vector2 operator*(float scale,const Vector2& value);
	Vector2 operator/(const Vector2& value, float scale);

	// 3�����x�N�g��
	struct Vector3
	{
		float x;
		float y;
		float z;

		Vector3();
		Vector3(float mx, float my, float mz);
		Vector3(const Vector3& other);
		Vector3(Vector3&& other);
		Vector3(const Vector4& other);
		Vector3(const Quaternion& other);
		Vector3 operator-() const;
		Vector3 operator+() const;
		Vector3& operator=(const Vector3& value);
		Vector3& operator+=(const Vector3& value);
		Vector3& operator-=(const Vector3& value);
		Vector3& operator*=(float scale);
		Vector3& operator/=(float scale);

		/// �x�N�g���̑傫�����擾����
		float Length() const;

		/// �x�N�g���̑傫����2��l���擾����
		float LengthSquare() const;
	};

	bool operator==(const Vector3& t1, const Vector3& t2);
	bool operator!=(const Vector3& t1, const Vector3& t2);
	Vector3 operator+(const Vector3& t1, const Vector3& t2);
	Vector3 operator-(const Vector3& t1, const Vector3& t2);
	Vector3 operator*(const Vector3& value, float scale);
	Vector3 operator*(float scale, const Vector3& value);
	Vector3 operator/(const Vector3& value, float scale);
	float Dot(const Vector3& v1, const Vector3& v2);
	Vector3 Cross(const Vector3& v1, const Vector3& v2);
	Vector3 Normalize(const Vector3& vec);

	/// 2�̃x�N�g���Ԋp�x���v�Z����
	float CalcAngleVecToVec(const Vector3& v1, const Vector3& v2);


	// 4�����x�N�g��
	struct Vector4
	{
		float x;
		float y;
		float z;
		float w;

		Vector4();
		Vector4(float mx, float my, float mz, float mw);
		Vector4(const Vector3& other);
		Vector4(const Vector4& other);
		Vector4(Vector4&& other);
		Vector4 operator-() const;
		Vector4 operator+() const;
		Vector4& operator=(const Vector4& value);
		Vector4& operator+=(const Vector4& value);
		Vector4& operator-=(const Vector4& value);
		Vector4& operator*=(float scale);
		Vector4& operator/=(float scale);
	};
	bool operator==(const Vector4& t1, const Vector4& t2);
	bool operator!=(const Vector4& t1, const Vector4& t2);
	Vector4 operator+(const Vector4& t1, const Vector4& t2);
	Vector4 operator-(const Vector4& t1, const Vector4& t2);
	Vector4 operator*(const Vector4& value, float scale);
	Vector4 operator*(float scale, const Vector4& value);
	Vector4 operator/(const Vector4& value, float scale);

	/// @struct Matrix4x4
	/// 4x4�s���\���\����
	/// �l�����x�N�g��v��4x4�s��M�Ƃ̏�Z��
	/// v*M�ŉ��Z�\�ŁAVector4��Ԃ�
	struct Matrix4x4
	{
		union
		{
			float matrix[4][4];

			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
		};

		Matrix4x4();
		Matrix4x4(const Matrix4x4& mat);
		Matrix4x4(Matrix4x4&& mat);
		Matrix4x4(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44);

		float& operator()(unsigned int row, unsigned int col);
		const float& operator()(unsigned int row, unsigned int col) const;

		Matrix4x4 operator+() const;
		Matrix4x4 operator-() const;
		Matrix4x4& operator= (const Matrix4x4& value);
		Matrix4x4& operator+= (const Matrix4x4& value);
		Matrix4x4& operator-= (const Matrix4x4& value);
		Matrix4x4& operator*= (const Matrix4x4& value);
		Matrix4x4& operator*= (float scale);
		Matrix4x4& operator/= (float scale);

		// �s�񎮌v�Z
		float Determinant() const;
	};
	bool operator==(const Matrix4x4& mat1, const Matrix4x4& mat2);
	bool operator!=(const Matrix4x4& mat1, const Matrix4x4& mat2);
	Matrix4x4 operator+(const Matrix4x4& mat1, const Matrix4x4& mat2);
	Matrix4x4 operator-(const Matrix4x4& mat1, const Matrix4x4& mat2);
	Matrix4x4 operator*(const Matrix4x4& mat1, const Matrix4x4& mat2);
	Matrix4x4 operator*(const Matrix4x4& mat, float scale);
	Matrix4x4 operator*(float scale, const Matrix4x4& mat);
	Vector3 operator*(const Vector3& vec, const Matrix4x4& mat);
	Vector4 operator*(const Vector4& vec, const Matrix4x4& mat);
	Matrix4x4 operator/(const Matrix4x4& mat, float scale);

	/// 4x4�s�񂩂�3�����̊g�k�������擾����
	Vector3 GetMatrixScale(const Matrix4x4& mat);

	/// 4x4�s�񂩂�3�����̕��s�ړ��������擾����
	Vector3 GetMatrixTranslate(const Matrix4x4& mat);

	/// 4x4�s�񂩂�C�ӎ���]���s���N�H�[�^�j�I���������擾����
	Quaternion GetMatrixRotation(const Matrix4x4& mat);

	/// 4x4�s��̓]�u�s����擾����
	Matrix4x4 GetTransposeMatrix(const Matrix4x4& mat);

	/// 4x4�s��̋t�s����擾����
	Matrix4x4 GetInvertMatrix(const Matrix4x4& mat);

	/// 4x4�P�ʍs����쐬����
	Matrix4x4 CreateIdent();

	/// ����̃X�P�[���Ŋg�k����4x4�s����쐬����
	Matrix4x4 CreateScaleMatrix(float scale);
	Matrix4x4 CreateScaleMatrix(float sx, float sy, float sz);
	Matrix4x4 CreateScaleMatrix(const Vector3& scale);

	/// �w�肵�����������s�ړ����s��4x4�s����쐬����
	Matrix4x4 CreateTranslateMatrix(float tx, float ty, float tz);
	Matrix4x4 CreateTranslateMatrix(const Vector3& movement);

	/// X�����S��]���s��4x4�s����擾����
	Matrix4x4 CreateXRotMatrix(float rad);

	/// Y�����S��]���s��4x4�s����擾����
	Matrix4x4 CreateYRotMatrix(float rad);

	/// Z�����S��]���s��4x4�s����擾����
	Matrix4x4 CreateZRotMatrix(float rad);

	/// �C�ӎ��𒆎~�ɉ�]���s��4x4�s����擾����
	Matrix4x4 CreateAxisRotMatrix(const Vector3& axis,float rad);

	/// ���_�A�����_�A������x�N�g������LookAt�s����쐬����
	Matrix4x4 CreateLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& upper);

	/// �J�����s�񂩂�LookAt�s����쐬����
	Matrix4x4 CreateLookAtMatrixFromCameraMatrix(const Matrix4x4& cameraMatrix);

	/// �A�X�y�N�g��A�O�ʃN���b�v�ʁA����N���b�v�ʁA��p����p�[�X�y�N�e�B�u�s����쐬����
	Matrix4x4 CreatePerspectiveMatrix(float Aspect, float nearZ, float farZ, float fov);


	/// @struct Matrix4x3
	/// 4x3�s���\���\����
	/// 4�����܂���3�����x�N�g��v��4x3�s��M�Ƃ̏�Z��
	/// v*M�ŉ��Z�\�ŁAVector3��Ԃ�
	struct Matrix4x3
	{
		union
		{
			float matrix[4][3];

			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
				float _41, _42, _43;
			};
		};
		
		Matrix4x3();
		Matrix4x3(const Matrix4x3& mat);
		Matrix4x3(const Matrix4x4& mat);
		Matrix4x3(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33,
			float m41, float m42, float m43);

		float& operator()(unsigned int row, unsigned int col);
		const float& operator()(unsigned int row, unsigned int col) const;

		Matrix4x3 operator+() const;
		Matrix4x3 operator-() const;
		Matrix4x4 operator=(const Matrix4x4& value);
	};
	

	struct Quaternion
	{
		union
		{
			struct
			{
				float x, y, z;
			};

			Vector3 v;
		};
		float w;

		Quaternion();
		Quaternion(float mw, float mx, float my, float mz);
		Quaternion(float mx, float my, float mz);
		Quaternion(const Math::Vector3& vector);
		Quaternion(const Quaternion& other);
		Quaternion(Quaternion&& other);

		Quaternion operator+() const;
		Quaternion operator-() const;
		Quaternion& operator=(const Quaternion& quat);
		Quaternion& operator+=(const Quaternion& quat);
		Quaternion& operator-=(const Quaternion& quat);
		Quaternion& operator*=(const Quaternion& quat);
		Quaternion& operator*=(float scale);
		Quaternion& operator/=(const Quaternion& quat);
		Quaternion& operator/=(float scale);
		float Norm() const;
		float NormSquare() const;
	};

	bool operator==(const Quaternion& q1, const Quaternion& q2);
	bool operator!=(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator*(const Quaternion& quat, float scale);
	Quaternion operator*(float scale, const Quaternion& quat);
	Quaternion operator/(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator/(const Quaternion& quat, float scale);

	/// �t�l�������쐬����
	Quaternion CreateInvertQuaternion(const Quaternion& quat);

	/// �����l�������쐬����
	Quaternion CreateConjugateQuaternion(const Quaternion& quat);

	/// �C�ӎ�������]���s���l�������쐬����
	Quaternion CreateRotAxisQuaternion(const Vector3& axis, float angle);

	/// XYZ����]���s���l�������쐬����
	Quaternion CreateRotXYZQuaternion(const Vector3& rotAngle);

	/// R3���[���h���W�n�ŔC�ӂ̕����x�N�g����
	/// �C�ӂ̕����x�N�g���։�]������l�������쐬����
	Quaternion CreateRotVecToVec(const Vector3& dstVec, const Vector3& srcVec, const Vector3& up = Vector3(0.0f,1.0f,0.0f));

	/// 2�̃N�H�[�^�j�I������`��Ԃ���
	/// @param[in] Quaternion q1
	/// @param[in] Quaternion q2
	/// @note q1����q2�ɂ����ĕ�Ԃ��s���B
	/// t=0.0f�̂Ƃ��Aq1
	/// t = 1.0f�̂Ƃ��Aq2
	/// �������At�́@0.0f <= t <= 1.0f�ƂȂ�悤�ɃN�����v�����
	Quaternion Lerp(const Quaternion& q1, const Quaternion q2, float t);

	/// 2�̃N�H�[�^�j�I�������ʐ��`��Ԃ���
	Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);

	/// 2�̃N�H�[�^�j�I���̓���
	float Dot(const Quaternion& q1, const Quaternion& q2);

	/// 2�̃N�H�[�^�j�I���Ԃ̊p�x�����߂�
	float CalcAngleQuatToQuat(const Quaternion& q1, const Quaternion& q2);

	/// �N�H�[�^�j�I�������]�s������߂�
	Matrix4x4 GetMatrixFromQuat(const Quaternion& quat);

}
