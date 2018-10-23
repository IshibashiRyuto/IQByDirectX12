/*
	IndexBuffer.h
	インデックス情報を格納するバッファ
	@author Ishibashi Ryuto
	@param history
	2018/08/28		初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// 自作ヘッダインクルード

// クラス前方宣言
using Microsoft::WRL::ComPtr;
class Device;

class IndexBuffer
{
public:
	~IndexBuffer();

	/// @fn Create
	/// 頂点インデックスバッファの生成
	/// @note IndexBufferクラスはこのメソッドによってのみ生成可能
	/// @param[in] device: ID3D12Device
	/// @param[in] pIndexResource: インデックスリソースのポインタ
	/// @param[in] indexCount: インデックスの数
	/// @param[in] indexSize: インデックス1つ当たりのサイズ(sizeofで取得)
	/// @retval 生成成功: VertexBufferのshared_ptr, 生成失敗: nullptr
	static std::shared_ptr<IndexBuffer> Create(std::shared_ptr<Device> device, void* pIndexResource, size_t indexCount, size_t indexSize);

	const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView();

	unsigned int GetIndexCount();
private:
	IndexBuffer();
	unsigned int			mIndexCount;
	ComPtr<ID3D12Resource>	mIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
};

