/**
*	@file	Dx12Camera.h
*	@brief	Dx12Cameraクラスの宣言を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/10/23	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "Camera.h"

/* クラス使用宣言 */
class Device;
class ConstantBuffer;
class DescriptorHeap;
class GraphicsCommandList;
class RootSignature;

/**
*	@class	Dx12Camera
*	@brief	Dx12で使用するためのカメラクラス
*			自身で定数バッファ、ヒープを持ち、
*			描画時のヒープセットまで行う
*/
class Dx12Camera :
	public Camera
{
public:
	/**
	*	@brief コンストラクタ
	*
	*	@param[in]	pos			: 初期位置座標
	*	@param[in]	direction	: 初期カメラ向き
	*	@param[in]	projType	: 投影方法
	*	@param[in]	projParam	: 投影パラメータ
	*/
	Dx12Camera(
		const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam,
		std::shared_ptr<Device> device);

	/**
	*	@brief デストラクタ
	*/
	~Dx12Camera();

	/**
	*	@brief		dx12カメラクラスを生成する
	*
	*	@param[in]	pos			: 初期位置
	*	@param[in]	direction	: 初期方向
	*	@param[in]	projType	: 投影方法
	*	@param[in]	projParam	: 投影パラメータ
	*	@param[in]	device		: dx12デバイス
	*/
	static std::shared_ptr<Dx12Camera> Create(
		const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam,
		std::shared_ptr<Device> device);

	/**
	*	@brief	行列情報を更新する
	*/
	void UpdateMatrix();

	/**
	*	@brief	ワールド変換行列をセットする
	*
	*	@param[in]	worldMat	: ワールド変換行列
	*/
	void SetWorldMatrix(const Math::Matrix4x4& worldMat);

	/**
	*	@brief	デスクリプタをルートシグネチャにセットする
	*
	*	@param[in]	rootSignature	: バインド対象のルートシグネチャ
	*	@param[in]	rootParamIndex	: バインドするルートパラメータのインデックス
	*/
	void BindDescriptorHeap(std::shared_ptr<RootSignature> rootSignature, int rootParamIndex);

private:
	Math::Matrix4x4 mWorldMatrix;						//! World行列
	std::shared_ptr<ConstantBuffer> mCameraDataBuffer;	//! カメラ情報格納定数バッファ
	std::shared_ptr<DescriptorHeap> mCameraHeap;		//! カメラ情報格納ヒープ
	
	/**
	*	@struct	CameraData
	*	@brief	シェーダに渡すカメラ情報
	*/
	struct CameraData
	{
		Math::Matrix4x4 wvpMat;		//! WVP変換行列
		Math::Matrix4x4 worldMat;	//! World行列
		Math::Matrix4x4 viewMat;	//! View行列
		Math::Matrix4x4 projMat;	//! Projection行列
		Math::Vector4	eyePos;		//! 視点
		Math::Vector4	eyeDir;		//! 視線ベクトル
		Math::Vector4	upper;		//! 上方向ベクトル
		Math::Vector4	right;		//! 右方向ベクトル
	};
};

