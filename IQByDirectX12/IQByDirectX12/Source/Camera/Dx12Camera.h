#pragma once
#include "Camera.h"

/* クラス使用宣言 */
class Device;
class ConstantBuffer;
class DescriptorHeap;
class GraphicsCommandList;

/// @brief Dx12で使用するためのカメラクラス
///	自身で定数バッファ、ヒープを持ち、
/// 描画時のヒープセットまで行う
class Dx12Camera :
	public Camera
{
public:
	/// @brief コンストラクタ
	Dx12Camera(
		const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam,
		std::shared_ptr<Device> device);

	/// @brief デストラクタ
	~Dx12Camera();

	/// @brief dx12カメラクラスを生成する
	/// @param[in]	pos			: 初期位置
	/// @param[in]	direction	: 初期方向
	/// @param[in]	projType	: 射影方法
	/// @param[in]	projParam	: 射影方法のパラメータ
	/// @param[in]	device		: dx12デバイス
	static std::shared_ptr<Dx12Camera> Create(
		const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam,
		std::shared_ptr<Device> device);

	/// @brief	行列情報を更新する
	void UpdateMatrix();

	/// @brief ワールド変換行列をセットする
	/// @param[in]	worldMat	: ワールド変換行列
	void SetWorldMatrix(const Math::Matrix4x4& worldMat);

	/// @brief バッファ情報をコマンドに乗せる
	/// @param[in]	commandList		: コマンドリスト
	/// @param[in]	rootParamIndex	: バッファをセットするルートパラメータのインデックス
	void SetCameraData(std::shared_ptr<GraphicsCommandList> commandList, int rootParamIndex);

private:
	Math::Matrix4x4 mWorldMatrix;	//! World行列
	std::shared_ptr<ConstantBuffer> mCameraDataBuffer;	//! カメラ情報格納バッファ
	std::shared_ptr<DescriptorHeap> mCameraHeap;	//! カメラ情報ヒープ
	
	/// @brief シェーダに渡すカメラ情報
	struct CameraData
	{
		Math::Matrix4x4 wvpMat;		//! WVP変換行列
		Math::Matrix4x4 worldMat;	//! World行列
		Math::Matrix4x4 viewMat;	//! View行列
		Math::Matrix4x4 projMat;	//! Projection行列
		Math::Vector3	eyePos;		//! 視点
		Math::Vector3	eyeDir;		//! 視線ベクトル
		Math::Vector3	upper;		//! 上方向ベクトル
		Math::Vector3	right;		//! 右方向ベクトル
	};
};

