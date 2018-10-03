/*
	Device.h
	D3D12Device実体を格納する
	実体はComPtrで格納されており、ComPtrでのやり取りのみを許可している
	(生ポインタはComPtrから取得する。直接取得することは許さない)
	@author Ishibashi Ryuto
	@history
	2018/07/11 初版作成
*/
#pragma once
/* システムヘッダインクルード */
#include <memory>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

// ComPtr使用宣言
using Microsoft::WRL::ComPtr;

class Device
{
public:

	/// デストラクタ
	~Device();

	/// @fn Create
	/// デバイスを作成する
	/// @note デバイスはこのメソッドで生成する。Deviceの実体をそのまま持つことはできない。
	/// @retval デバイス作成成功時:Deviceのスマートポインタ, デバイス作成失敗時:nullptr
	static std::shared_ptr<Device> Create();

	/// @fn GetDevice
	/// デバイスのComPtrを取得する
	/// @retval ComPtr(ID3D12Device) デバイスのComPtr
	ComPtr<ID3D12Device> GetDevice() const;

	/// @fn GetFeatureLevel
	/// デバイスを作成したフィーチャーレベルを取得する
	D3D_FEATURE_LEVEL GetFeatureLevel() const;

private:
	/// コンストラクタ
	Device();

	D3D_FEATURE_LEVEL mFeatureLevel;
	ComPtr<ID3D12Device> mDevice;
};

