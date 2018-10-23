/*
	@file Camera.h
	@brief ゲーム内のカメラ情報を格納する
	@author Ishibashi Ryuto
	@date 2018/10/21	初版作成
*/
#pragma once
/* システムヘッダインクルード */
#include <memory>
#include <map>
#include <functional>

/* 自作ヘッダインクルード */
#include "../Math/Math.h"

enum class ProjectionType
{
	Perspective,
	Orthographic
};

/// @brief 射影行列の設定パラメータ
struct ProjectionParam
{
	float width;
	float height;
	float fov;
	float nearZ;
	float farZ;
};

/// @brief カメラ情報を管理する
class Camera
{
public:
	/// @brief	コンストラクタ
	Camera(const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam);

	/// @brief	デストラクタ
	~Camera();

	/// @brief	カメラクラスを生成する
	/// @note	カメラクラスはこのメソッドを通してのみ生成可能
	/// @param[in]	pos			: 初期位置
	/// @param[in]	direction	: 初期方向
	/// @param[in]	projType	: 射影方法
	/// @param[in]	projParam	: 射影方法のパラメータ
	static std::shared_ptr<Camera> Create(const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam);

	/// @brief	カメラの注視点を指定する
	/// @param[in]	targetPos :カメラの注視点
	void SetTargetPos(const Math::Vector3& targetPos);

	/// @brief	カメラの視点(位置)を指定する
	/// @param[in]	pos	: カメラの位置
	void SetPos(const Math::Vector3& pos);

	/// @brief	カメラの方向を指定する
	/// @param[in]	direction	: カメラの向き
	void SetDirection(const Math::Vector3& direction);

	/// @brief	カメラの方向を指定する
	/// @param[in]	direction	: z軸正向きからのカメラの回転
	void SetDirection(const Math::Quaternion& direction);

	/// @brief	カメラの射影方法を指定する
	void SetProjectionType(ProjectionType projType, const ProjectionParam& projParam);

	/// @brief	カメラを回転させる
	/// @param[in]	rotation	: 回転量
	void Rotate(const Math::Quaternion& rotation);

	/// @brief	カメラをアッパーベクトルとともに回転させる
	void RotateWithUpper(const Math::Quaternion& rotation);

	/// @brief	カメラを移動させる
	/// @param[in]	movement	: 移動量
	void Move(const Math::Vector3& movement);

	/// @brief	カメラ行列を更新する
	void UpdateMatrix();

	/// @brief	ビュー行列を取得する
	const Math::Matrix4x4& GetViewMatrix() const;
	
	/// @brief	プロジェクション行列を取得する
	const Math::Matrix4x4& GetProjMatrix() const;

private:
	/* 変数宣言 */
	Math::Vector3 mPosition;		//! カメラの位置
	Math::Quaternion mDirection;	//! カメラの向き
	Math::Vector3 mUpper;			//! カメラの上方向ベクトル
	Math::Matrix4x4 mViewMat;		//! ビュー行列
	Math::Matrix4x4 mProjMat;		//! プロジェクション行列
	std::map<ProjectionType, std::function<Math::Matrix4x4(const ProjectionParam&)>> CreateProjMat;	//! 射影行列生成を行う関数

	/* ローカルメソッド定義 */
	
	/// @brief	透視投影行列を作成する
	static Math::Matrix4x4 CreatePerspectiveMatrix(const ProjectionParam& projParam);

	/// @brief	平行投影行列を作成する
	static Math::Matrix4x4 CreateOrthographicMatrix(const ProjectionParam& projParam);

	/// @brief	ビュー行列を更新する
	void UpdateViewMat();
};

