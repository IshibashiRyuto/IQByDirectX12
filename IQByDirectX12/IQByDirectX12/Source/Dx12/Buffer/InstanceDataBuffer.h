/**
*	@file	InstanceDataBuffer.h
*	@brief	インスタンシング用データバッファクラスを記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/21	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "Dx12Resource.h"

/**
*	@class	InstanceDataBuffer
*	@brief	インスタンシング用データを格納するクラス
*
*	インスタンシング用情報を格納するクラス
*	テンプレート化したかったが管理上の問題が発生したため断念
*/
class InstanceDataBuffer :
	public Dx12Resource
{
public:
	/**
	*	@biref	コンストラクタ
	*	@param[in]	device				: dx12デバイス
	*	@param[in]	instanceDataSize	: インスタンスデータ一つ当たりのサイズ
	*	@param[in]	maxInstanceCount	: 最大インスタンス数
	*	@param[in]	bufferName			: バッファ名
	*	@param[out]	result				: バッファの生成結果
	*/
	InstanceDataBuffer(std::shared_ptr<Device> device,
		size_t instanceDataSize,
		size_t maxInstanceCount,
		const std::wstring& bufferName,
		HRESULT& result);

	/**
	*	@biref	デストラクタ
	*/
	~InstanceDataBuffer();

	/**
	*	@brief	インスタンスデータ用バッファを作成する
	*
	*	@param[in]	device				: dx12デバイス
	*	@param[in]	instanceDataSize	: インスタンスデータのサイズ
	*	@param[in]	maxInstanceCount	: 最大インスタンス数
	*	@param[in]	bufferName			: バッファ名
	*/
	static std::shared_ptr<InstanceDataBuffer> Create(std::shared_ptr<Device> device,
		size_t instanceDataSize,
		size_t maxInstanceCount,
		const std::wstring& bufferName = L"");

	/**
	*	@brief	インスタンスデータを登録する
	*	
	*	インスタンスデータの登録を行う。
	*	インスタンスデータのサイズは、バッファ作成時のデータサイズを参照する
	*	また、最大インスタンス数を超えたデータは破棄される
	*/
	void SetInstanceData(void* pInstanceData, int instanceDataIndex);


	/**
	*	@biref	頂点バッファビューを取得する
	*/
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const;
private:
	/* 変数宣言 */
	unsigned int mMaxInstanceCount;				//! 最大インスタンス数
	void* m_pInstanceBuffer;					//! インスタンスバッファのポインタ
	size_t mInstanceDataSize;					//! インスタンスのデータサイズ
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;	//! 頂点バッファビュー

	/**
	*	@brief	頂点バッファビューを構築する
	*	@param[in]	dataSize	: データサイズ
	*	@param[in]	dataCount	: データ数
	*/
	void ConstructVertexBufferView(size_t dataSize, size_t dataCount);

	/**
	*	@biref	バッファをマップする
	*/
	void Map();
};

