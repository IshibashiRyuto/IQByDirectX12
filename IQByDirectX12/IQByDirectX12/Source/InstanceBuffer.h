/*
	InstanceBuffer.h
	インスタンス情報格納用のバッファ
	@author Ishibashi Ryuto
	@date
	2018/09/04	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// 自作ヘッダインクルード

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class Device;

class InstanceBuffer
{
public:
	// コンストラクタ
	InstanceBuffer();

	// デストラクタ
	~InstanceBuffer();

	/// @fn Create
	/// インスタンスバッファを作成する
	/// @note InstanceBufferクラスはこのメソッドを通じてのみ生成可能
	/// @param[in] device : ID3D12デバイス
	/// @param[in] instanceDataSize	: インスタンスデータのサイズ
	/// @param[in] maxInstanceCount	: 最大インスタンス数
	static std::shared_ptr<InstanceBuffer> Create(std::shared_ptr<Device> device, size_t instanceDataSize, int maxInstanceCount);

	/// @fn GetVertexBufferView
	/// 頂点バッファビューの取得
	/// @retvval D3D12_VERTEX_BUFFER_VIEW 頂点バッファビューへの参照
	const D3D12_VERTEX_BUFFER_VIEW & GetVertexBufferView() const;

	/// @fn SetInstanceData
	/// インスタンスデータをセットする
	/// @param[in] pInstanceData		: インスタンスデータへのポインタ
	/// @param[in] instanceDataIndex	: 何番目のインスタンスデータをセットするか
	/// @note	インスタンスデータのサイズはインスタンスバッファ生成時のものを参照
	///			あとから変更することは不可能
	void SetInstanceData(void* pInstanceData, int instanceDataIndex);

private:
	int mMaxInstanceCount;							// 最大インスタンス数
	ComPtr<ID3D12Resource> mInstanceBuffer;			// インスタンスバッファリソース
	int mInstanceDataSize;							// インスタンスデータのサイズ
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;		// 頂点バッファビュー
	void* pInstanceBuffer;							// インスタンスバッファのポインタ
};

