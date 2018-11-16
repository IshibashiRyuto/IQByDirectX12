/*
*	@file	Dx12Resource.h
*	@brief	ID3D12Resourceのラッピングクラスを記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/16	初版作成
*/

#pragma once
/* ヘッダインクルード */
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <string>

/* クラス使用宣言 */
using Microsoft::WRL::ComPtr;
class Device;

///	@class	Dx12Resource
/// @brief	ID3D12Resourceのラッパークラス
///	@note	Createでオブジェクトを作成、newは不可
class Dx12Resource
{
public:
	/// @brief	コンストラクタ
	/// @param[in]	device		: dx12デバイス
	///	@param[in]	heapFlag	: ヒープフラグ
	/// @param[in]	heapProp	: ヒーププロパティ
	/// @param[in]	desc		: リソースデスク
	/// @param[in]	state		: リソースの初期ステート
	/// @param[in]	clearValue	: クリア値
	/// @param[out]	result		: 
	Dx12Resource(
		std::shared_ptr<Device> device,
		const D3D12_HEAP_PROPERTIES& heapProp,
		D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		D3D12_RESOURCE_STATES states,
		const D3D12_CLEAR_VALUE& clearValue,
		HRESULT& result);

	/// @brief	デストラクタ
	~Dx12Resource();

	///	@brief	Dx12Resourceオブジェクトを生成する
	///
	/// @param[in]	device		: dx12デバイス
	/// @param[in]	heapProp	: ヒーププロパティ
	///	@param[in]	heapFlag	: ヒープフラグ
	/// @param[in]	desc		: リソースデスク
	/// @param[in]	state		: リソースの初期ステート
	/// @param[in]	clearValue	: クリア値
	///
	/// @retval	生成成功	: Dx12Resourceのスマートポインタ
	/// @retval	生成失敗	: nullptr
	std::shared_ptr<Dx12Resource> Create(
		std::shared_ptr<Device> device,
		const D3D12_HEAP_PROPERTIES& heapProp,
		D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		D3D12_RESOURCE_STATES states,
		const D3D12_CLEAR_VALUE& clearValue,
		const std::wstring& resourceName = L""
	);

	/// @brief	リソース実体を取得する
	/// @retval	リソース実体のCOMポインタ
	const ComPtr<ID3D12Resource> GetResource() const;

private:

	/* 変数宣言 */
	ComPtr<ID3D12Resource> mResource;	//! リソース実体
};

