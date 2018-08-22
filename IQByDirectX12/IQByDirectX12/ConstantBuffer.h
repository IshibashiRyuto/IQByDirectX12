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
	/// コンストラクタ
	ConstantBuffer();

	/// デストラクタ
	~ConstantBuffer();


	/// @fn Create
	/// 定数バッファを作成する
	/// @note ConstantBufferクラスはこのクラスを用いてのみ生成可能
	/// @param[in] device ID3D12DeviceのComPtr
	/// @param[in] bufferSize	バッファのサイズ
	/// @retval 生成成功時: ConstantBufferのshared_ptr
	/// @retval 生成失敗時: nullptr
	static std::shared_ptr<ConstantBuffer> Create(const ComPtr<ID3D12Device> device, UINT bufferSize);


private:
	ComPtr<ID3D12Resource> mBuffer;
	D3D12_CONSTANT_BUFFER_VIEW_DESC mCBVDesc;
};

