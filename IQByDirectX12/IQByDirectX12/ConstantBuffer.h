/*
	ConstantBuffer.h
	定数バッファクラス
	@author Ishibashi Ryuto
	@param history
	2017/07/31	初版作成
*/
#pragma once
/*システムヘッダインクルード*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*自作ヘッダインクルード*/

/*クラス前方宣言*/
using Microsoft::WRL::ComPtr;

class ConstantBuffer
{
public:
	/// デストラクタ
	~ConstantBuffer();


	/// @fn Create
	/// 定数バッファを作成する
	/// @note ConstantBufferクラスはこのクラスを用いてのみ生成可能
	/// @param[in] device ID3D12DeviceのComPtr
	/// @param[in] bufferSize	バッファのサイズ
	/// @retval 生成成功時: ConstantBufferのshared_ptr
	/// @retval 生成失敗時: nullptr
	static std::shared_ptr<ConstantBuffer> Create(const ComPtr<ID3D12Device> device, UINT elementSize, UINT elementCount = 1);

	/// @fn SetData
	/// 定数バッファにデータをセットする
	/// @param[in] data データのポインタ
	/// @param[in] elementSize データサイズ
	/// @param[in] elementNum 何番目の要素を更新するか
	/// @retval データサイズよりバッファサイズが大きい場合: true
	/// @retval データサイズよりバッファサイズが小さい場合: false
	bool SetData(const void* data, UINT elementSize, UINT elementIndex = 0);

	/// @fn GetConstantBufferView
	/// コンスタントバッファビューを取得する
	/// @retval コンスタントバッファビュー
	const D3D12_CONSTANT_BUFFER_VIEW_DESC & GetConstantBufferView(UINT elementIndex = 0);

private:
	ComPtr<ID3D12Resource> mBuffer;				// バッファ
	UINT mElementSize;							// 1要素当たりのバッファサイズ
	UINT mElementCount;							// 要素数
	D3D12_CONSTANT_BUFFER_VIEW_DESC mCBVDesc;	// コンスタントバッファビュー
	char* mBufAddress;

	/// コンストラクタ
	ConstantBuffer(UINT elementSize, UINT elementCount);

	/// @fn CreateConstantBuffer
	/// コンスタントバッファの実体を作成する
	/// @param[in] device ID3D12DeviceのComPtr
	/// @param[in] bufferSize	バッファのサイズ
	/// @retval 作成成功時: true
	/// @retval 作成失敗時: false
	bool CreateConstantBuffer(const ComPtr<ID3D12Device> device);

	/// @fn CreateConstantBufferView
	/// コンスタントバッファビューの作成を行う
	/// @retval 生成成功時:	true
	/// @retval 生成失敗時: false
	bool CreateConstantBufferView();
};

