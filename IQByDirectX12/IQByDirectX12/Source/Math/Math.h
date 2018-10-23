/*
	@file Math.h
	@brief 自作数学関連ライブラリ
	@author Ishibashi Ryuto
	@history 
	2018/07/19 初版作成
	2018/08/26 行列クラスに機能を追加
		・LookAt行列の生成
		・Perspective行列の生成
	2018/10/03	データ量削減のため4x3行列を追加
	2018/10/22	Perspective行列,OrthoGraphic行列,単位クォータニオン生成関数を追加
*/
#pragma once
namespace Math
{
	// 構造体前方宣言
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix4x4;
	struct Matrix4x3;
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

	// 2次元ベクトル
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

	// 3次元ベクトル
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

		/// ベクトルの大きさを取得する
		float Length() const;

		/// ベクトルの大きさの2乗値を取得する
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

	/// 2つのベクトル間角度を計算する
	float CalcAngleVecToVec(const Vector3& v1, const Vector3& v2);


	// 4次元ベクトル
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
	/// 4x4行列を表す構造体
	/// 四次元ベクトルvと4x4行列Mとの乗算は
	/// v*Mで演算可能で、Vector4を返す
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
		Matrix4x4(const Matrix4x3& mat);
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

		// 行列式計算
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

	/// 4x4行列から3次元の拡縮成分を取得する
	Vector3 GetMatrixScale(const Matrix4x4& mat);

	/// 4x4行列から3次元の並行移動成分を取得する
	Vector3 GetMatrixTranslate(const Matrix4x4& mat);

	/// 4x4行列から任意軸回転を行うクォータニオン成分を取得する
	Quaternion GetMatrixRotation(const Matrix4x4& mat);

	/// 4x4行列の転置行列を取得する
	Matrix4x4 GetTransposeMatrix(const Matrix4x4& mat);

	/// 4x4行列の逆行列を取得する
	Matrix4x4 GetInvertMatrix(const Matrix4x4& mat);

	/// 4x4単位行列を作成する
	Matrix4x4 CreateIdent();

	/// 特定のスケールで拡縮する4x4行列を作成する
	Matrix4x4 CreateScaleMatrix(float scale);
	Matrix4x4 CreateScaleMatrix(float sx, float sy, float sz);
	Matrix4x4 CreateScaleMatrix(const Vector3& scale);

	/// 指定した分だけ平行移動を行う4x4行列を作成する
	Matrix4x4 CreateTranslateMatrix(float tx, float ty, float tz);
	Matrix4x4 CreateTranslateMatrix(const Vector3& movement);

	/// X軸中心回転を行う4x4行列を取得する
	Matrix4x4 CreateXRotMatrix(float rad);

	/// Y軸中心回転を行う4x4行列を取得する
	Matrix4x4 CreateYRotMatrix(float rad);

	/// Z軸中心回転を行う4x4行列を取得する
	Matrix4x4 CreateZRotMatrix(float rad);

	/// 任意軸を中止に回転を行う4x4行列を取得する
	Matrix4x4 CreateAxisRotMatrix(const Vector3& axis,float rad);

	/// 視点、注視点、上方向ベクトルからLookAt行列を作成する
	Matrix4x4 CreateLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& upper);

	/// 視点、向き、上方向ベクトルからLookAt行列を作成する
	/// @note 向きはz軸正向きをデフォルトとする回転
	Matrix4x4 CreateLookAtMatrix(const Vector3& eyePos, const Quaternion& direction, const Vector3& upper);

	/// カメラ行列からLookAt行列を作成する
	Matrix4x4 CreateLookAtMatrixFromCameraMatrix(const Matrix4x4& cameraMatrix);

	/// アスペクト比、前面クリップ面、後方クリップ面、画角からパースペクティブ行列を作成する
	Matrix4x4 CreatePerspectiveMatrix(float Aspect, float nearZ, float farZ, float fov);

	/// アスペクト比、前面クリップ面、後方クリップ面から平行投影行列を作成する
	Matrix4x4 CreateOrthoGraphicMatrix(float width, float height, float nearZ, float farZ);


	/// @struct Matrix4x3
	/// 4x3行列を表す構造体
	/// 4次元または3次元ベクトルvと4x3行列Mとの乗算は
	/// v*Mで演算可能で、Vector3を返す
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
		Matrix4x3& operator=(const Matrix4x3& value);
		Matrix4x3& operator+=(const Matrix4x3& value);
		Matrix4x3& operator-=(const Matrix4x3& value);
		Matrix4x3& operator*=(const Matrix4x3& value);
		Matrix4x3& operator*=(float scale);
		Matrix4x3& operator/=(float scale);

		/// 行列式計算
		float Determinant() const;
	};

	bool operator==(const Matrix4x3& mat1, const Matrix4x3& mat2);
	bool operator!=(const Matrix4x3& mat1, const Matrix4x3& mat2);
	Matrix4x3 operator+(const Matrix4x3& mat1, const Matrix4x3& mat2);
	Matrix4x3 operator-(const Matrix4x3& mat1, const Matrix4x3& mat2);
	Matrix4x3 operator*(const Matrix4x3& mat1, const Matrix4x3& mat2);
	Matrix4x3 operator*(const Matrix4x3& mat, float scale);
	Matrix4x3 operator*(float scale, const Matrix4x3& mat);
	Vector3 operator*(const Vector3& vec, const Matrix4x3& mat);
	Vector4 operator*(const Vector4& vec, const Matrix4x3& mat);
	Matrix4x4 operator/(const Matrix4x3& mat, float scale);
	

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

	/// 単位クォータニオンを作成する
	Quaternion CreateIdentQuaternion();

	/// 逆四元数を作成する
	Quaternion CreateInvertQuaternion(const Quaternion& quat);

	/// 共役四元数を作成する
	Quaternion CreateConjugateQuaternion(const Quaternion& quat);

	/// 任意軸方向回転を行う四元数を作成する
	Quaternion CreateRotAxisQuaternion(const Vector3& axis, float angle);

	/// XYZ軸回転を行う四元数を作成する
	Quaternion CreateRotXYZQuaternion(const Vector3& rotAngle);

	/// R3ワールド座標系で任意の方向ベクトルを
	/// 任意の方向ベクトルへ回転させる四元数を作成する
	Quaternion CreateRotVecToVec(const Vector3& dstVec, const Vector3& srcVec, const Vector3& up = Vector3(0.0f,1.0f,0.0f));

	/// 2つのクォータニオンを線形補間する
	/// @param[in] Quaternion q1
	/// @param[in] Quaternion q2
	/// @note q1からq2にかけて補間を行う。
	/// t=0.0fのとき、q1
	/// t = 1.0fのとき、q2
	/// ただし、tは　0.0f <= t <= 1.0fとなるようにクランプされる
	Quaternion Lerp(const Quaternion& q1, const Quaternion q2, float t);

	/// 2つのクォータニオンを球面線形補間する
	Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);

	/// 2つのクォータニオンの内積
	float Dot(const Quaternion& q1, const Quaternion& q2);

	/// 2つのクォータニオン間の角度を求める
	float CalcAngleQuatToQuat(const Quaternion& q1, const Quaternion& q2);

	/// クォータニオンから回転行列を求める
	Matrix4x4 GetMatrixFromQuat(const Quaternion& quat);

}
