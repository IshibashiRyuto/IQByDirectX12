/**
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
class GraphicsCommandList;

/**	
*	@class	Dx12Resource
*	@brief	ID3D12Resourceのラッパークラス
*	@note	Createでオブジェクトを作成、newは不可
*/
class Dx12Resource
{
public:
	/// @brief	デストラクタ
	virtual ~Dx12Resource();

	/**
	*	@brief	Dx12Resourceオブジェクトを生成する
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	heapProp	: ヒーププロパティ
	*	@param[in]	heapFlag	: ヒープフラグ
	*	@param[in]	desc		: リソースデスク
	*	@param[in]	state		: リソースの初期ステート
	*	@param[in]	clearValue	: クリア値(nullptr可)
	*
	*	@retval	生成成功	: Dx12Resourceのスマートポインタ
	*	@retval	生成失敗	: nullptr
	*/
	static std::shared_ptr<Dx12Resource> Create(
		std::shared_ptr<Device> device,
		const D3D12_HEAP_PROPERTIES& heapProp,
		D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		D3D12_RESOURCE_STATES states,
		const D3D12_CLEAR_VALUE * pClearValue,
		const std::wstring& resourceName = L""
	);

	/**
	*	@brief	リソース実体を取得する
	*	@retval	リソース実体のCOMポインタ
	*/
	const ComPtr<ID3D12Resource> GetResource() const;

	/**
	*	@brief	リソース状態遷移を行う
	*
	*	@param[in]	commandList		: 遷移バリアを載せるコマンドリスト
	*	@param[in]	transitionState	: 遷移先状態
	*/
	void TransitionState(std::shared_ptr<GraphicsCommandList> commandList, D3D12_RESOURCE_STATES transitionState);


protected:
	/**
	*	@brief	コンストラクタ
	*	
	*	@param[in]	device			: dx12デバイス
	*	@param[in]	heapProp		: ヒーププロパティ
	*	@param[in]	heapFlag		: ヒープフラグ
	*	@param[in]	desc			: リソースデスク
	*	@param[in]	state			: リソースの初期ステート
	*	@param[in]	clearValue		: クリア値
	*	@param[in]	resourceName	: リソース名
	*	@param[out]	result			: バッファの生成に成功したか
	*/
	Dx12Resource(
		std::shared_ptr<Device> device,
		const D3D12_HEAP_PROPERTIES& heapProp,
		D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		D3D12_RESOURCE_STATES states,
		const D3D12_CLEAR_VALUE* pClearValue,
		const std::wstring& resourceName,
		HRESULT& result);

	/**
	*	@brief	コンストラクタ
	*	@param[in]	resource	: 構築済みリソースバッファ
	*/
	Dx12Resource(std::shared_ptr<Dx12Resource> resource);

	/**
	*	@brief	コンストラクタ
	*	@param[in]	resource	: 構築済みID3D12Resource
	*	@param[in]	state		: リソースのステート
	*/
	Dx12Resource(ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES state);

	/* 変数宣言 */
	ComPtr<ID3D12Resource> mResource;		//! リソース実体
	D3D12_RESOURCE_STATES mCurrentState;	//! リソースの状態
};

