/*
	VertexBuffer.h
	頂点バッファクラス
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class VertexBuffer
{
public:
	/// デストラクタ
	~VertexBuffer();

	/// @fn Create
	/// 頂点バッファの生成
	/// @note VertexBufferクラスはこのメソッドによってのみ生成可能
	/// @param[in] device: ID3D12Device
	/// @param[in] pVertexResource: 頂点リソースのポインタ
	/// @param[in] vertexCount: 頂点数
	/// @param[in] vertexSize: 1頂点当たりのサイズ(sizeofで取得)
	/// @retval 生成成功: VertexBufferのshared_ptr, 生成失敗: nullptr
	static std::shared_ptr<VertexBuffer> Create(ComPtr<ID3D12Device> device, void* pVertexResource, size_t vertexCount, size_t vertexSize);

	/// @fn GetVertexBufferView
	/// 頂点バッファビューの取得
	/// @retval D3D12_VERTEX_BUFFER_VIEW 頂点バッファビューへの参照
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView();

private:
	/// コンストラクタ
	VertexBuffer();
	VertexBuffer(const VertexBuffer&);

	ComPtr<ID3D12Resource> mVertexBuffer;			// 頂点バッファリソース
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;		// 頂点バッファビュー
};