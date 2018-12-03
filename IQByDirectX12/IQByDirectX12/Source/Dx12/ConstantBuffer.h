/*
	@file	ConstantBuffer.h
	@brief	ConstantBufferの宣言を記述する
	@author Ishibashi Ryuto
	@date	2018/07/31	初版作成
	@date	2018/12/03	CreateConstantBufferViewの返り値をbool->voidに変更(必ず成功するため意味がなかった)
*/
#pragma once
/* ヘッダインクルード */
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*クラス前方宣言*/
using Microsoft::WRL::ComPtr;
class Device;

/**
*	@class	ConstantBuffer 
*	@brief	定数バッファクラス
*
*	シェーダリソースとして渡す定数バッファクラス
*	一つのバッファの別領域を参照してデータを受け渡すことが可能
*/
class ConstantBuffer
{
public:
	/**
	*	@brief	デストラクタ
	*/
	~ConstantBuffer();

	/**
	*	@brief	定数バッファを作成する
	*
	*	@param[in] device		: Deviceクラス
	*	@param[in] bufferSize	: バッファのサイズ
	*
	*	@retval 生成成功時		: ConstantBufferのshared_ptr
	*	@retval 生成失敗時		: nullptr
	*
	*	@note ConstantBufferクラスはこのクラスを用いてのみ生成可能
	*/
	static std::shared_ptr<ConstantBuffer> Create(std::shared_ptr<Device> device, UINT elementSize, UINT elementCount = 1);

	/**
	*	@brief	定数バッファにデータをセットする
	*
	*	@param[in] data データのポインタ
	*	@param[in] elementSize データサイズ
	*	@param[in] elementNum 何番目の要素を更新するか
	*
	*	@retval データサイズよりバッファサイズが大きい場合: true
	*	@retval データサイズよりバッファサイズが小さい場合: false
	*/
	bool SetData(const void* data, UINT elementSize, UINT elementIndex = 0);

	/**
	*	@brief	コンスタントバッファビューを取得する
	*	@retval コンスタントバッファビュー
	*/
	const D3D12_CONSTANT_BUFFER_VIEW_DESC & GetConstantBufferView(UINT elementIndex = 0);

private:
	ComPtr<ID3D12Resource> mBuffer;				//! バッファ実体
	UINT mElementSize;							//! 1要素当たりのバッファサイズ
	UINT mElementCount;							//! 要素数
	D3D12_CONSTANT_BUFFER_VIEW_DESC mCBVDesc;	//! コンスタントバッファビュー
	char* mBufAddress;

	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	elementSize		: 1要素当たりのデータサイズ
	*	@param[in]	elementCount	: 要素数
	*/
	ConstantBuffer(UINT elementSize, UINT elementCount);

	/**
	*	@brief	コンスタントバッファの実体を作成する
	*	@param[in]	device		: ID3D12DeviceのComPtr
	*	@param[in]	bufferSize	: バッファのサイズ
	*	@retval 作成成功時: true
	*	@retval 作成失敗時: false
	*/
	bool CreateConstantBuffer(const ComPtr<ID3D12Device> device);

	/**
	*	@brief	コンスタントバッファビューの作成を行う
	*/
	void CreateConstantBufferView();
};

