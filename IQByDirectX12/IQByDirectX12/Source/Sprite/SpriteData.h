/*
*	@file	SpriteData.h
*	@brief	スプライトデータを格納するクラス
*	@author	Ishibashi Ryuto
*	@date	2018/11/20	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include <memory>
#include "../Math/Math.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../Dx12/DescriptorHeap.h"


/* クラス使用宣言 */
struct InstanceData;
class PipelineStateObject;
class GraphicsCommandList;


/**
*	@class	SpriteData
*	@brief	スプライトデータを管理するクラス
*/
class SpriteData
{
public:
	/**
	*	@brief	コンストラクタ
	*	@param[in]	vertexBuffer	: 頂点バッファ情報
	*	@param[in]	descHeap		: デスクリプタヒープ
	*	@param[in]	pso				: パイプラインステートオブジェクト
	*/
	SpriteData(std::shared_ptr<VertexBuffer> vertexBuffer,
		std::shared_ptr<DescriptorHeap> descHeap,
		std::shared_ptr<PipelineStateObject> pso);

	/**
	*	@brief	デストラクタ
	*/
	~SpriteData();

	/**
	*	@brief	スプライトデータを作成する
	*/
	static std::shared_ptr<SpriteData> Create(std::shared_ptr<VertexBuffer> vertexBuffer,
		std::shared_ptr<DescriptorHeap> descHeap,
		std::shared_ptr<PipelineStateObject> pso);

	/**
	*	@biref	更新処理
	*/
	void Update();

	/**
	*	@brief	描画処理
	*	@param[in]	commandList		: 描画対象のコマンドリスト
	*	@param[in]	InstanceData	: インスタンシング用データ
	*/
	void Draw(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

private:
	std::shared_ptr<VertexBuffer>	mVertexBuffer;	//! 頂点バッファ
	std::shared_ptr<DescriptorHeap>	mDescHeap;		//! デスクリプタヒープ
	std::shared_ptr<PipelineStateObject> mPSO;		//! パイプラインステートオブジェクト
};

