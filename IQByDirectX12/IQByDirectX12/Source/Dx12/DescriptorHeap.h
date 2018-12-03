/*
	@file	DescriptorHeap.h
	@brief	DescriptorHeapクラスの宣言を記述する
	@author	Ishibashi Ryuto
	@date	2018/08/23以前(詳細不明)	初版作成
	@date	2018/11/24	DSV,RTV用ヒープの作成に対応
						DSV,RTVのセットに対応
	@date	2018/11/30	BindRootParameter, SetBindHeapIndexを追加
						伴ってルートパラメータのバインド方法を変更
*/
#pragma once
/*ヘッダインクルード*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>


/* クラス使用宣言 */
using Microsoft::WRL::ComPtr;
class Texture;
class RenderTargetTexture;
class Device;
class GraphicsCommandList;

/**
*	@class	DescriptorHeap
*	@brief	デスクリプタヒープクラス
*
*	シェーダにリソースの読み取り方法を教えるための情報である
*	Descriptorを格納するDescriptorHeapのラッパークラス
*	Descriptorはデータの見方をViewという形で具象化しており、
*	そのViewをHeapに格納していき、ルートパラメータにバインドして読み取っていく
*	そのため、バインド先のルートパラメータのインデックス等を情報として持つ
*/
class DescriptorHeap
{
public:
	/**
	*	@brief	デストラクタ
	*/
	~DescriptorHeap();

	/**
	*	@brief	ディスクリプタヒープの作成
	*
	*	@param[in]	device		: デバイス
	*	@param[in]	desc		: デスクリプタヒープの設定
	*
	*	@retval		生成成功	: DescriptorHeapのポインタ
	*	@retval		生成失敗	: nullptr
	*
	*	@note DescriptorHeapはこのメソッドを通じてのみ作成可能
	*/
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

	/**
	*	@brief	ディスクリプタヒープの作成
	*
	*	@param[in] device			: デバイス
	*	@param[in] descriptorNum	: セット可能なデスクリプタの個数
	*
	*	@retval		生成成功		: DescriptorHeapのポインタ
	*	@retval		生成失敗		: nullptr
	*
	*	@note DescriptorHeapはこのメソッドを通じてのみ作成可能
	*/
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, UINT numDescriptors);

	/**
	*	@brief	ディスクリプタヒープの作成
	*	
	*	@param[in]	device			: dx12デバイス
	*	@param[in]	type			: ディスクリプタヒープタイプ
	*	@param[in]	descriptorsNum	: セット可能なデスクリプタの個数
	*	@param[in]	flag			: ヒープフラグ
	*
	*	@retval		生成成功		: DescriptorHeapのポインタ
	*	@retval		生成失敗		: nullptr
	*/
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, unsigned int descriptorsNum, D3D12_DESCRIPTOR_HEAP_FLAGS flag);

	/**
	*	@brief	コンスタントバッファビューをセットする
	*
	*	@param[in] constantBufferView	: コンスタントバッファビュー
	*	@param[in] constantBuffer		: コンスタントバッファ
	*	@param[in] index				: ヒープのインデックス
	*/
	void SetConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& constantBufferView, UINT index);

	/**
	*	@brief	シェーダリソースビューをセットする
	*
	*	@param[in]	shaderResourceView	: シェーダリソースビュー
	*	@param[in]	shaderResource		: シェーダリソース
	*	@param[in] index				: ヒープのインデックス
	*/
	void SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& shaderResourceView, ComPtr<ID3D12Resource> shaderResource, UINT index);

	/**
	*	@brief	テクスチャ情報をセットする
	*
	*	@param[in]	texture		: テクスチャ
	*	@param[in]	index		: ヒープのインデックス
	*/
	void SetTexture(std::shared_ptr<Texture> texture, UINT index);

	/**
	*	@brief	テクスチャ情報をセットする
	*
	*	@param[in]	texture		: テクスチャ
	*	@param[in]	index		: ヒープのインデックス
	*/
	void SetTexture(std::shared_ptr<RenderTargetTexture> texture, UINT index);

	/**
	*	@brief	構造化バッファビューをセットする
	*
	*	@param[in]	unorderedAccessView	構造化バッファビュー
	*	@param[in]	structuredBuffer	構造化バッファ
	*	@param[in] index				ヒープのインデックス
	*/
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
	*
	*	@param[in]	commandList	: バインド対象のコマンドリスト
	*/
	void BindGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> commandList);

	/**
	*	@brief	ディスクリプタテーブルとヒープをバインドする
	*
	*	@param[in]	rootParamIndex		: ルートパラメータのインデックス
	*	@param[in]	descriptorHeapIndex	:	ヒープ側のインデックス
	*/
	void BindRootDescriptorTable(unsigned int rootParamIndex, unsigned int descriptorHeapIndex);

	/**
	*	@brief	ヒープをルートパラメータにバインドする
	*
	*	@param[in]	commandList		: バインドするコマンドリスト
	*	@param[in]	rootParamIndex	: バインドするルートパラメータインデックス
	*
	*	@note		バインドするヒープ側のインデックスは事前に設定しておく
	*/
	void BindRootParameter(std::shared_ptr<GraphicsCommandList> commandList, unsigned int rootParamIndex);

	/**
	*	@brief	ルートパラメータにバインドする、ヒープのインデックスを指定する
	*	
	*	@param[in]	heapIndex	: バインドするヒープ位置
	*/
	void SetBindHeapIndex(unsigned int heapIndex);

	/**
	*	@brief	指定したインデックスのGPUハンドルを取得する
	*
	*	@param[in]	index	: 取得したいいハンドルのインデックス
	*
	*	@return	GPUハンドル
	*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(UINT index) const;

	/**
	*	@brief	指定したインデックスのCPUハンドルを取得する
	*
	*	@param[in]	index	: 取得したいハンドルのインデックス
	*
	*	@return	CPUハンドル
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT index) const;
private:
	/*定数宣言*/
	const UINT HEAP_STRIDE;										//! ヒープ一つ当たりの幅

	/*変数定義*/
	std::shared_ptr<Device>			mDevice;					//! デバイス
	ComPtr<ID3D12DescriptorHeap>	mDescriptorHeap;			//! ディスクリプタヒープの実態
	UINT							mNumDescriptors;			//! ディスクリプタの数
	ComPtr<ID3D12GraphicsCommandList>	mGraphicsCommandList;	//!	描画コマンドリスト
	unsigned int mHeapIndex;									//! セットするヒープのインデックス

	/*ローカルメソッド*/

	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	heapDesc	: ヒープ構築情報
	*	@param[out]	result		: ヒープの構築に成功したか
	*/
	DescriptorHeap(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc, HRESULT& result);

};

