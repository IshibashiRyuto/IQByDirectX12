#pragma once
namespace XMMath
{
	//	�\���̑O���錾
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix4x4;
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


}