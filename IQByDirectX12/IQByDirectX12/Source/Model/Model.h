/*
	@file	Model.h
	@brief	Modelクラスの宣言を記述する
	@author Ishibashi Ryuto
	@date
	2018/09/03	初版作成
	2018/11/28	同一ハンドルを持つモデルの作成を外部から行えるようにした
				モデルの複製を行うメソッドを追加
				(複製したメソッドは同一モデルでも別インスタンス扱い)
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "../Math/Math.h"

// クラス使用宣言
class ModelDataManager;
class CommandList;
class DescriptorHeap;
class Pose;
using Microsoft::WRL::ComPtr;
class InstancingDataManager;

/**
*	@class	Model
*	@brief	ゲーム内のモデル毎に扱う情報の管理と描画等を行う
*/
class Model
{
public:
	/// コンストラクタ
	Model();
	Model(int modelHandle);

	/// デストラクタ
	~Model();

	/// @fn Create
	/// モデルクラスを生成する
	/// @param[in] modelHandle	: モデルハンドル
	/// @retval モデルのスマートポインタ
	static std::shared_ptr<Model> Create(int modelHandle);

	/**
	*	@brief	モデルクラスを生成する
	*
	*	@param[in]	model	: 既存のモデルデータ
	*/
	static std::shared_ptr<Model> Create(std::shared_ptr<Model> model);

	/**
	*	@brief	モデルを複製する
	*
	*	@param[in]	model	: 複製するモデルデータ
	*
	*	@return		複製されたモデルデータ
	*
	*	TODO	: 一時実装保留
	*/


	/**
	*	@brief	位置を指定する
	*
	*	@param[in] position : モデル配置位置
	*/
	void SetPosition(const Math::Vector3& position);

	/**
	*	@brief	モデルの回転を指定する
	*
	*	@param[in] rotation : 各軸回転角度
	*/
	void SetRotation(const Math::Vector3& rotation);

	/**
	*	モデルの回転を指定する
	*	@param[in] rotQuaternion : 各軸回転角度(quaternion)
	*/
	void SetRotation(const Math::Quaternion& rotQuaternion);

	/**
	*	モデルの拡縮率を指定する
	*	@param[in]	 scale	: モデルスケール
	*/
	void SetScale(const Math::Vector3& scale);

	/**
	*	モデルの拡縮率を指定する
	*	@param[in]	 scale	: モデルスケール
	*/
	void SetScale(float scale);

	/**
	*	@brief	モデルの描画処理を行う
	*/
	void Draw() const;

	/**
	*	@brief	デバッグ用のデスクリプタヒープ取得
	*/
	std::shared_ptr<DescriptorHeap> _DebugGetDescHeap();

	/**
	*	@brief	ポーズを取得する
	*/
	std::shared_ptr<Pose> _DebugGetPose();

private:
	// 変数宣言
	Math::Vector3		mPosition;					//! 位置
	Math::Quaternion	mRotation;					//! 回転
	Math::Vector3		mScale;						//! 拡縮
	Math::Matrix4x4		mModelMatrix;				//! モデル行列
	int					mModelHandle;				//! モデルハンドル
	InstancingDataManager& mInstancingDataManager;	//! インスタンシングデータのマネージャ

	// ローカルメソッド定義
	/**
	*	@brief	現在の位置、回転、拡縮からモデル行列を計算する
	*/
	void CalcModelMatrix();
};

