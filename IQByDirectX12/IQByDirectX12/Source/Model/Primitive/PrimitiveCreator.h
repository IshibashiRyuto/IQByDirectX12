/**
*	@file	PrimitiveCreator.h
*	@brief	PrimitiveCreatorの宣言を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/29	初版作成
*/
#pragma once
#include <memory>
#include "../Math/Math.h"
#include "PrimitiveData.h"

/* クラス使用宣言 */
class Device;
class Model;
class PipelineStateObject;
class RootSignature;

/**
*	@class	PrimitiveCreator
*	@brief	プリミティブモデル生成を管理するクラス
*/
class PrimitiveCreator
{
public:
	~PrimitiveCreator();

	/**
	*	@brief	インスタンスを取得する
	*/
	static PrimitiveCreator& GetInstance()
	{
		static PrimitiveCreator inst;
		return inst;
	}

	/**
	*	@brief	初期化処理
	*
	*	@param[in]	device			: dx12デバイス
	*	@param[in]	pipelineState	: プリミティブ用パイプラインステート
	*	@param[in]	rootSignature	: プリミティブ用ルートシグネチャ
	*/
	void Initialize(std::shared_ptr<Device> device, std::shared_ptr<PipelineStateObject> pipelineState,
		std::shared_ptr<PipelineStateObject> shadowPSO, std::shared_ptr<RootSignature> rootSignature);

	/**
	*	@brief	床を生成する
	*	
	*	@param[in]	size		: 床のサイズ
	*	@param[in]	material	: マテリアル
	*/
	std::shared_ptr<Model> CreatePlane(const Math::Vector2& size, const Primitive::Material& material = Primitive::Material());

private:
	/**
	*	@brief	コンストラクタ
	*/
	PrimitiveCreator();

	/**
	*	@brief	コピーコンストラクタ
	*/
	PrimitiveCreator(PrimitiveCreator &){}

	/**
	*	@brief	コピー代入の禁止
	*/
	void operator=(PrimitiveCreator& ) { }
	
	bool mIsInit{ false };									//! 初期化処理が終了しているか
	std::shared_ptr<Device> mDevice;						//! デバイス
	std::shared_ptr<PipelineStateObject> mPipelineState;	//! プリミティブ用パイプラインステート
	std::shared_ptr<RootSignature> mRootSignature;			//! ルートシグネチャ
	std::shared_ptr<PipelineStateObject> mShadowPSO;		//! 深度書き込み用
};

