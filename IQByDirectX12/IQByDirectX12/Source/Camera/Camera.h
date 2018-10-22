/*
	@file Camera.h
	@brief ゲーム内のカメラ情報を格納する
	@author Ishibashi Ryuto
	@date 2018/10/21	初版作成
*/
#pragma once

/* 自作ヘッダインクルード */
#include "../Math/Math.h"

enum class ProjectionType
{
	Perspective,
	Orthographic,
};

/// @brief 射影行列の設定パラメータ
struct ProjectionParam
{
	float near;
	float far;
	float aspect;
	float fov;
};

/// @brief カメラ情報を管理する
class Camera
{
public:
	Camera();
	~Camera();

	/// @brief カメラの注視点を指定する
	/// @param[in]	targetPos :カメラの注視点
	void SetTargetPos(const Math::Vector3& targetPos);

	/// @brief カメラの視点(位置)を指定する
	/// @param[in]	pos	: カメラの位置
	void SetPos(const Math::Vector3& pos);

	/// @brief カメラの方向を指定する
	/// @param[in]	direction	: カメラの向き
	void SetDirection(const Math::Vector3& direction);

	/// @brief カメラの方向を指定する
	/// @param[in]	direction	: z軸正向きからのカメラの回転
	void SetDirection(const Math::Quaternion& direction);

	/// @brief カメラの射影方法を指定する
	void SetProjection(ProjectionType projType, const ProjectionParam& projParam);

private:
	Math::Vector3 mPosition;		//! カメラの位置
	Math::Vector3 mDirection;		//! カメラの向き
	Math::Matrix4x4 mViewMat;		//! ビュー行列
	Math::Matrix4x4 mProjMat;		//! プロジェクション行列
};

