/*
	@file	DescriptorHeap.h
	@brief	DescriptorHeapクラスを記述する
	@author	Ishibashi Ryuto
	@date	2018/??/??	初版作成
	@date	2018/11/24	DSV,RTV用ヒープの作成に対応
						DSV,RTVのセットに対応
*/
#pragma once
/*システムヘッダインクルード*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*自作ヘッダインクルード*/

/*前方宣言*/
using Microsoft::WRL::ComPtr;
class Texture;
class RenderTargetTexture;
class Device;

class DescriptorHeap
{
public:
	/// @brief	デストラクタ
	~DescriptorHeap();

	/// @brief	ディスクリプタヒープの作成
	/// @note DescriptorHeapはこのメソッドを通じてのみ作成可能
	/// @param[in]	device		デバイス
	/// @param[in]	desc		デスクリプタヒープの設定
	/// @retval		生成成功	DescriptorHeapのポインタ
	/// @retval		生成失敗	nullptr
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

	/// @brief	ディスクリプタヒープの作成
	/// @note DescriptorHeapはこのメソッドを通じてのみ作成可能
	/// @param[in] device			デバイス
	/// @param[in] descriptorNum	セット可能なデスクリプタの個数
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, UINT numDescriptors);

	/**
	*	@brief	ディスクリプタヒープの作成
	*	
	*	@param[in]	device			: dx12デバイス
	*	@param[in]	type			: ディスクリプタヒープタイプ
	*	@param[in]	descriptorsNum	: セット可能なデスクリプタの個数
	*	@param[in]	flag			: ヒープフラグ
	*/
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, unsigned int descriptorsNum, D3D12_DESCRIPTOR_HEAP_FLAGS flag);

	/**
	*	@brief	コンスタントバッファビューをセットする
	*	@param[in] constantBufferView	コンスタントバッファビュー
	*	@param[in] constantBuffer		コンスタントバッファ
	*	@param[in] index				ヒープのインデックス
	*/
	void SetConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& constantBufferView, UINT index);

	/// @brief	シェーダリソースビューをセットする
	/// @param[in]	shaderResourceView	シェーダリソースビュー
	/// @param[in]	shaderResource		シェーダリソース
	/// @param[in] index				ヒープのインデックス
	void SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& shaderResourceView, ComPtr<ID3D12Resource> shaderResource, UINT index);

	/// @brief	テクスチャ情報をセットする
	/// @param[in]	texture			テクスチャ
	/// @param[in]	index			ヒープのインデックス
	void SetTexture(std::shared_ptr<Texture> texture, UINT index);

	/// @brief	テクスチャ情報をセットする
	/// @param[in]	texture			テクスチャ
	/// @param[in]	index			ヒープのインデックス
	void SetTexture(std::shared_ptr<RenderTargetTexture> texture, UINT index);

	/// @brief	構造化バッファビューをセットする
	/// @param[in]	unorderedAccessView	構造化バッファビュー
	/// @param[in]	structuredBuffer	構造化バッファ
	/// @param[in] index				ヒープのインデックス
	void SetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& unorderedAccessView, ComPtr<ID3D12Resource> structuredBuffer, UINT index);

	/**
	*	@brief	デプスステンシルビューをセットする
	*
	*	@param[in]	depthStencilViewDesc	: デプスステンシルビュー情報
	*	@param[in]	depthBuffer				: 深度バッファ
	*	@param[in]	index					: セット先のインデックス
	*/
	void SetDepthStencilView(const D3D12_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc, ComPtr<ID3D12Resource> resource, UINT index);

	/**
	*	@brief	デスクリプタヒープをコマンドリストにバインドする
	*/
	void BindGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> commandList);

	/**
	*	@brief	ディスクリプタテーブルとヒープをバインドする
	*	@param[in]	rootParamIndex	: ルートパラメータのインデックス
	*	@param[in]	descriptorHeapIndex	:	ヒープ側のインデックス
	*/
	void BindRootDescriptorTable(int rootParamIndex, int descriptorHeapIndex);

	/**
	*	@brief	指定したインデックスのGPUハンドルを取得する
	*	@param[in]	index	: 取得したいいハンドルのインデックス
	*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(UINT index) const;

	/**
	*	@brief	指定したインデックスのCPUハンドルを取得する
	*	@param[in]	index	: 取得したいハンドルのインデックス
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT index) const;
private:
	/*定数宣言*/
	const UINT HEAP_STRIDE;								//! ヒープ一つ当たりの幅

	/*変数定義*/
	std::shared_ptr<Device>			mDevice;			//! デバイス
	ComPtr<ID3D12DescriptorHeap>	mDescriptorHeap;	//! ディスクリプタヒープの実態
	UINT							mNumDescriptors;		//! ディスクリプタの数
	ComPtr<ID3D12GraphicsCommandList>	mGraphicsCommandList;	//!	描画コマンドリスト

	/*ローカルメソッド*/

	/// コンストラクタ
	DescriptorHeap(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc, HRESULT& result);

};

