/*
*	@file	PrimitiveModelData.h
*	@brief	プリミティブのモデルデータクラスを記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/19	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "../ModelData.h"
#include "PrimitiveData.h"

/* クラス使用宣言 */
class ConstantBuffer;

/**
*	@class	PrimitiveModelData
*	@brief	プリミティブモデルのベースクラス
*/
class PrimitiveModelData :
	public ModelData
{
public:
	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	device				: dx12デバイス
	*	@param[in]	vertices			: 頂点情報
	*	@param[in]	indexes				: インデックス情報
	*	@param[in]	material			: 単一マテリアル情報
	*	@param[in]	pipelineStateObject	: パイプラインステートオブジェクト
	*	@param[in]	rootSignature		: ルートシグネチャ
	*/
	PrimitiveModelData(std::shared_ptr<Device> device,
		const std::vector<Primitive::Vertex>& vertices,
		const std::vector<Primitive::Index>& indexes,
		const Primitive::Material& material,
		std::shared_ptr<PipelineStateObject> pipelineStateObject,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);

	/**
	*	@brief	デストラクタ
	*/
	virtual ~PrimitiveModelData();

	/**
	*	@brief	更新処理
	*/
	void Update();

	/**
	*	@brief	通常描画処理
	*
	*	@param[in]	commandList		: 描画コマンドを載せるコマンドリスト
	*	@param[in]	isntanceData	: インスタンスデータ
	*/
	void Draw(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

	/**
	*	@brief	シャドウ描画処理
	*
	*	@param[in]	commandList		: 描画コマンドを載せるコマンドリスト
	*	@param[in]	isntanceData	: インスタンスデータ
	*/
	void DrawShadow(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

protected:
	std::shared_ptr<ConstantBuffer> mMaterialBuf;		//! マテリアル用定数バッファ
	Primitive::Material mMaterial;						//! マテリアル
};

