/**
*	@file	VertexBuffer_Def.h
*	@brief	頂点バッファクラスの定義を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/21	初版作成
*/

#pragma once
/* ヘッダインクルード */
#include "Dx12Resource.h"
#include <vector>

/**
*	@class	VertexBuffer
*	@brief	頂点バッファクラス
*
*	頂点バッファ情報を格納するクラス
*	テンプレートにしたかったが、実体化しなければ
*	他のテンプレートクラスに格納できなくて困る場面があったので
*	データの格納はユーザ管理に任せるようにします	
*/
class VertexBuffer :
	public Dx12Resource
{
public:

	/**
	*	@brief	デストラクタ
	*/
	virtual ~VertexBuffer();

	/**
	*	@brief	頂点バッファを生成する
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	pVertices	: 頂点情報
	*	@param[in]	vertexSize	: 1頂点当たりのサイズ(sizeofで取得)
	*	@param[in]	vertexCount	: 頂点数
	*	@param[in]	bufferName	: バッファ名
	*
	*	@retval		生成成功	: VertexBufferのスマートポインタ
	*	@retval		生成失敗	: nullptr
	*/
	static std::shared_ptr<VertexBuffer> Create
	(
		std::shared_ptr<Device> device,
		const void* pVertices,
		size_t vertexSize,
		size_t vertexCount,
		const std::wstring& bufferName = L""
	);

	/**
	*	@brief	頂点バッファビューを取得する
	*
	*	@return	頂点バッファビューへの参照
	*/
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const;

	/**
	*	@brief		バッファに情報を書き込む
	*
	*	@param[in]	data			: 書き込むデータ
	*	@param[in]	dataSize		: 書き込むデータのサイズ
	*	@param[in]	dataCount		: 書き込むデータの数
	*	@param[in]	dataIndexOffset	: 書き込み先のオフセットインデックス(dataSize参照)
	*
	*	バッファの先頭から(dataSize * dataIndexOffset)分進んだ位置に
	*	dataをdataSize分書き込む
	*/
	void WriteVertexBuffer(const void* data, size_t dataSize, size_t dataCount, unsigned int dataIndexOffset = 0);
protected:
	/* 変数宣言 */
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;	//! 頂点バッファビュー

	/**
	*	@brief	頂点バッファビューを構築する
	*
	*	@param[in]	dataSize	: データサイズ
	*	@param[in]	dataCount	: データ数
	*/
	void ConstructVertexBufferView(size_t dataSize, size_t dataCount);


	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	device		: デバイス
	*	@param[in]	dataSize	: データサイズ
	*	@param[in]	dataCount	: データ数
	*	@param[in]	bufferName	: バッファ名
	*	@param[out]	result		: バッファの作成結果
	*/
	VertexBuffer(std::shared_ptr<Device> device,
		size_t dataSize,
		size_t dataCount,
		const std::wstring& bufferName,
		HRESULT& result);
};

