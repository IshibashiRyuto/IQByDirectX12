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
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	vertices	: 頂点情報
	*	@param[in]	indexes		: インデックス情報
	*	@param[in]	material	: 単一マテリアル情報
	*/
	PrimitiveModelData(std::shared_ptr<Device> device,
		const std::vector<Primitive::Vertex>& vertices,
		const std::vector<Primitive::Index>& indexes,
		const Primitive::Material& material,
		std::shared_ptr<PipelineStateObject> pipelineStateObject);
	virtual ~PrimitiveModelData();

	void Update();

	void Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;
protected:
	std::shared_ptr<ConstantBuffer> mMaterialBuf;		//! マテリアル用定数バッファ
	Primitive::Material mMaterial;						//! マテリアル
};

