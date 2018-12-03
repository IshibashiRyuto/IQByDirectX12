/**
*	@file	PrimitivePlane.h
*	@brief	PrimitivePlaneの宣言を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/28	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "PrimitiveModelData.h"

/**
*	@class	PrimitivePlane
*	@brief	プリミティブの平面
*/
class PrimitivePlane :
	public PrimitiveModelData
{
public:
	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	device			: dx12デバイス
	*	@param[in]	vertices		: 頂点情報
	*	@param[in]	indexes			: 頂点インデックス情報
	*	@param[in]	material		: マテリアル情報
	*	@param[in]	pipelineState	: パイプラインステート
	*	@param[in]	rootSignature	: ルートシグネチャ
	*/
	PrimitivePlane(std::shared_ptr<Device> device,
		const std::vector<Primitive::Vertex>& vertices,
		const std::vector<short>& indexes,
		const Primitive::Material& material,
		std::shared_ptr<PipelineStateObject> pipelineState,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);
	~PrimitivePlane();

	/**
	*	@brief	床を生成する
	*	
	*	@param[in]	device			: 生成用デバイス
	*	@param[in]	size			: 床のサイズ
	*	@param[in]	material		: マテリアル
	*	@param[in]	pipelineState	: パイプラインステート
	*/
	static std::shared_ptr<PrimitivePlane> Create(
		std::shared_ptr<Device> device,
		const Math::Vector2& size,
		const Primitive::Material& material,
		std::shared_ptr<PipelineStateObject> pipelineState,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);

private:

	/**
	*	@brief	頂点情報を構築する
	*
	*	@param[in]	size		: 床のサイズ
	*	@param[out]	vertices	: 頂点情報格納先
	*	@param[out] indexes		: 頂点インデックス情報格納先
	*/
	static void ConstructVertices(const Math::Vector2& size, std::vector<Primitive::Vertex>& vertices, std::vector<short>& indexes);

};

