#pragma once
namespace XMMath
{
	//	構造体前方宣言
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix4x4;
	struct Quaternion;

	// 円周率π
	const float F_PI = 3.14159265f;

	// 円周率π(倍精度浮動小数点）
	const double D_PI = 3.141592653589793;

	/// @fn IsEqual
	/// 浮動小数点の等値比較を行う
	/// @retval true: 値が等しい, false: 値が等しくない
	bool IsEqual(float v1, float v2);
	bool IsEqual(double v1, double v2);
	bool IsEqual(float v1, double v2);
	bool IsEqual(double v1, float v2);

	/// @fn IsZero
	/// 浮動小数点のゼロ値判定を行う
	/// @retval true: 値が0である, false: 値が0でない
	bool IsZero(float value);
	bool IsZero(double value);

	/// @fn Clamp
	/// 値を指定した範囲内に収まるようにして返す
	/// @param[in] value: 対象となる値
	/// @param[in] minValue: 最小値
	/// @param[in] maxValue: 最大値
	/// @retval 範囲内にクランプされた値
	/// @note minValue > maxValue の場合、assert処理が入る
	float Clamp(float value, float minValue, float maxValue);
	double Clamp(double value, double minValue, double maxValue);


}