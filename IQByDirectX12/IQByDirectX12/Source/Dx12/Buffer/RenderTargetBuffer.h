/**
*	@file	RenderTargetBuffer.h
*	@brief	RenderTargetBufferの宣言を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/24	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "Dx12Resource.h"
#include "../Math/Math.h"

/**
*	@class	RenderTargetBuffer
*	@brief	レンダーターゲットとして使用可能なバッファを作成する
*/
class RenderTargetBuffer :
	public Dx12Resource
{
public:
	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	width		: 横幅
	*	@param[in]	height		: 縦幅
	*	@param[in]	format		: フォーマット
	*	@param[in]	clearValue	: クリアカラー
	*	@param[out] result		: バッファの生成結果
	*/
	RenderTargetBuffer(std::shared_ptr<Device> device,
		unsigned int width,
		unsigned int height,
		const std::wstring& bufferName,
		DXGI_FORMAT format,
		const D3D12_CLEAR_VALUE& clearValue,
		HRESULT & result);
	
	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	buffer	: 構築済みバッファ
	*	@param[in]	state	: 現在のバッファステート
	*/
	RenderTargetBuffer(ComPtr<ID3D12Resource> buffer, D3D12_RESOURCE_STATES state, const D3D12_CLEAR_VALUE& clearValue);

	/**
	*	@brief	デストラクタ
	*/
	~RenderTargetBuffer();

	/**
	*	@brief	RenderTargetBufferを作成する
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	width		: バッファ横幅
	*	@param[in]	height		: バッファ縦幅
	*	@param[in]	bufferName	: バッファ名
	*	@param[in]	format		: フォーマット
	*	@param[in]	clearColor	: バッファクリアカラー(デフォルトで黒)
	*
	*	@retval		生成成功	: RenderTargetBufferのスマートポインタ
	*	@retval		生成失敗	: nullptr
	*/
	static std::shared_ptr<RenderTargetBuffer> Create(std::shared_ptr<Device> device,
		unsigned int width,
		unsigned int height,
		const std::wstring& bufferName = L"",
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		Math::Vector4 clearColor = Math::Vector4(0.f, 0.f, 0.f, 1.f));

	/**
	*	@brief	RenderTargetBufferを作成する
	*
	*	@param[in]	buffer	: 構築済みバッファ
	*	@param[in]	state	: 現在のバッファステート
	*/
	static std::shared_ptr<RenderTargetBuffer> Create(ComPtr<ID3D12Resource> buffer, D3D12_RESOURCE_STATES state);

	/**
	*	@brief	RTV情報を取得する
	*/
	const D3D12_RENDER_TARGET_VIEW_DESC & GetRenderTargetViewDesc() const;


	/**
	*	@brief	クリア値を取得する
	*	
	*	@return	レンダーターゲットのクリア値
	*/
	const D3D12_CLEAR_VALUE& GetClearValue() const;

private:
	D3D12_RENDER_TARGET_VIEW_DESC mRTVDesc;		//! レンダーターゲットビュー情報
	const D3D12_CLEAR_VALUE CLEAR_VALUE;		//! クリア値

	/**
	*	@brief	レンダーターゲットビュー情報を構築する
	*/
	void ConstructRTVDesc();
};

