/*
*	@file	RenderTargetTexture.h
*	@brief	RenderTargetTextureクラスを記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/12	初版作成
*/

#pragma once

/* ヘッダインクルード */
#include "Texture.h"

/* クラス使用宣言 */
class Device;
class GraphicsCommandList;

/*
	@class	RenderTargetTexture
	@brief	レンダーターゲットとして使用可能なテクスチャ
*/
class RenderTargetTexture :
	public Texture
{
public:
	///	@brief	デストラクタ
	~RenderTargetTexture();

	///	@brief レンダーターゲットに指定可能なテクスチャを生成する
	///	@param[in]	device		: D3D12デバイスクラス
	///	@param[in]	width		: テクスチャの幅
	///	@param[in]	height		: テクスチャの高さ
	///	@param[in]	format		: レンダーターゲットのフォーマット(デフォルト:DXGI_FORMAT_R8G8B8A8_UNORM)
	///	@retval		生成成功	: RenderTargetTextureのshared_ptr
	///	@retval		生成失敗	: nullptr
	static std::shared_ptr<RenderTargetTexture> Create(std::shared_ptr<Device> device, int width, int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	///	@brief レンダーターゲットに指定可能なテクスチャを生成する
	/// @param[in]	buffer		: バッファ
	///	@retval		生成成功	: RenderTargetTextureのshared_ptr
	///	@retval		生成失敗	: nullptr
	static std::shared_ptr<RenderTargetTexture> Create(ComPtr<ID3D12Resource> buffer);
	
	/// @brief	レンダーターゲットビュー情報を取得する
	///	@retval	レンダーターゲットビュー情報
	const D3D12_RENDER_TARGET_VIEW_DESC & GetRenderTargetViewDesc() const;

	///	@brief	レンダーターゲットビュー情報をセットする
	void SetRenderTargetViewDesc(const D3D12_RENDER_TARGET_VIEW_DESC &rtvDesc);

	/// @brief	ステート状態を変更するリソースバリアを取得する
	///	@param[in]	dstState	: 遷移後のステート
	D3D12_RESOURCE_BARRIER GetTransitionBarrier(D3D12_RESOURCE_STATES dstState);

private:
	/* 変数宣言 */
	D3D12_RENDER_TARGET_VIEW_DESC mRTVDesc;			//! レンダーターゲットビュー情報
	D3D12_RESOURCE_STATES mCurrentResourceState;	//! 現在のリソースステート

	///	@brief	コンストラクタ
	RenderTargetTexture(ComPtr<ID3D12Resource> textureData);

	///	@brief	バッファ情報からレンダーターゲットビュー情報を構築する
	void ConstructRTVDesc();
};

