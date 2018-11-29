/**
*	@file	IndexBuffer.h
*	@brief	IndexBufferクラスの定義を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/22	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "Dx12Resource.h"

/**
*	@class	IndexBuffer
*	@brief	頂点インデックス情報を格納する
*/
class IndexBuffer :
	public Dx12Resource
{
public:

	/**
	*	@biref	デストラクタ
	*/
	~IndexBuffer();

	/**
	*	@biref	頂点インデックスバッファの作成
	*	
	*	@param[in]	device			: dx12デバイス
	*	@param[in]	pIndexResource	: インデックスリソースへのポインタ
	*	@param[in]	indexDataSize	: インデックスデータのサイズ(sizeofで取得した値を渡す)
	*	@param[in]	indexCount		: インデックスの数
	*	@param[in]	bufferName		: バッファ名
	*
	*	@retval		生成成功	: IndexBufferのスマートポインタ
	*	@retval		生成失敗	: nullptr
	*/
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<Device>device,
		const void* pVertexIndexes,
		size_t indexSize,
		size_t indexCount,
		const std::wstring& bufferName = L""
	);

	/**
	*	@biref	インデックスバッファビューを取得する
	*
	*	@retval	インデックスバッファビュー
	*/
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const;

	/**
	*	@brief		インデックスデータを書き込む
	*
	*	@param[in]	pIndexData		: インデックスデータへのポインタ
	*	@param[in]	indexDataSize	: インデックスデータ1つ当たりのサイズ
	*	@param[in]	dataCount		: 書き込むデータの数
	*	@param[in]	offsetIndex		: 書き込み先のオフセット
	*
	*	書き込み先アドレスの先頭から(indexDataSize * offsetIndex)分ずれた場所に
	*	(pIndexData)から(pIndexData + indexDataSize * dataCount)の範囲のデータを書きこむ
	*/
	void WriteIndexData(const void* pIndexData, size_t indexDataSize, size_t dataCount, unsigned int offsetIndex = 0);

	/**
	*	@brief	インデックス数を取得する
	*
	*	@return インデックス数
	*/
	unsigned int GetIndexCount() const;

private:
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;	//! 頂点インデックスバッファビュー
	unsigned int mIndexCount;

	/**
	*	@brief	インデックスバッファビューを構築する
	*
	*	@param[in]	indexDataSize	: インデックスデータ1つ当たりのサイズ
	*	@param[in]	indexCount		: インデックスの数
	*	@param[out]	result			: 構築結果
	*/
	void ConstructIndexBufferView(size_t indexDataSize, size_t indexCount, HRESULT& result);

	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	device			: dx12デバイス
	*	@param[in]	indexDataSize	: インデックスデータのサイズ
	*	@param[in]	indexCount		: インデックスの数
	*	@param[in]	bufferName		: バッファ名
	*	@param[out]	result			: バッファの生成結果
	*/
	IndexBuffer(
		std::shared_ptr<Device> device,
		size_t indexDataSize,
		size_t indexCount,
		const std::wstring& bufferName,
		HRESULT& result
	);
};

