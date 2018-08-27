/*
	DescriptorHeap.h
	ディスクリプタヒープ
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

class DescriptorHeap
{
public:
	/// デストラクタ
	~DescriptorHeap();

	/// @fn Create
	/// ディスクリプタヒープの作成
	/// @note DescriptorHeapはこのメソッドを通じてのみ作成可能
	/// @param[in]	device		デバイス
	/// @param[in]	desc		デスクリプタヒープの設定
	/// @retval		生成成功	DescriptorHeapのポインタ
	/// @retval		生成失敗	nullptr
	static std::shared_ptr<DescriptorHeap> Create(ComPtr<ID3D12Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

	/// @fn Create
	/// ディスクリプタヒープの作成
	/// @note DescriptorHeapはこのメソッドを通じてのみ作成可能
	/// @param[in] device			デバイス
	/// @param[in] descriptorNum	セット可能なデスクリプタの個数
	static std::shared_ptr<DescriptorHeap> Create(ComPtr<ID3D12Device> device, UINT numDescriptors);

	/// @fn SetConstantBufferView
	/// コンスタントバッファビューをセットする
	/// @param[in] constantBufferView	コンスタントバッファビュー
	/// @param[in] constantBuffer		コンスタントバッファ
	/// @param[in] index				ヒープのインデックス
	void SetConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& constantBufferView, UINT index);

	/// @fn SetShaderResourceView
	/// シェーダリソースビューをセットする
	/// @param[in]	shaderResourceView	シェーダリソースビュー
	/// @param[in]	shaderResource		シェーダリソース
	/// @param[in] index				ヒープのインデックス
	void SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& shaderResourceView, ComPtr<ID3D12Resource> shaderResource, UINT index);

	/// @fn SetTexture
	/// テクスチャ情報をセットする
	/// @param[in]	texture			テクスチャ
	/// @param[in]	index			ヒープのインデックス
	void SetTexture(std::shared_ptr<Texture> texture, UINT index);

	/// @fn UnorderedAccessView
	/// 構造化バッファビューをセットする
	/// @param[in]	unorderedAccessView	構造化バッファビュー
	/// @param[in]	structuredBuffer	構造化バッファ
	/// @param[in] index				ヒープのインデックス
	void SetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& unorderedAccessView, ComPtr<ID3D12Resource> structuredBuffer, UINT index);

	/// @fn BindGraphicsCommandList
	/// デスクリプタヒープをコマンドリストにバインドする
	void BindGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> commandList);

private:
	/*定数宣言*/
	const UINT HEAP_STRIDE;

	/*変数定義*/
	ComPtr<ID3D12Device>			mDevice;			//! デバイス
	ComPtr<ID3D12DescriptorHeap>	mDescriptorHeap;	//! ディスクリプタヒープの実態
	UINT							mNumDescriptors;		//! ディスクリプタの数

	/*ローカルメソッド*/

	/// コンストラクタ
	DescriptorHeap(ComPtr<ID3D12Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc);

};

