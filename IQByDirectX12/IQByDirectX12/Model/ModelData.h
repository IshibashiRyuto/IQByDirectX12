/*
	@file	ModelData.h
	@brief	モデルデータを格納するクラス
	@param	history
	2018/08/28	初版作成
*/
#pragma once
// システムヘッダインクルード
#include<memory>

//自作ヘッダインクルード
#include "../Math/Math.h"

// クラス使用宣言
class VertexBuffer;
class IndexBuffer;
class DescriptorHeap;
class ConstantBuffer;

class ModelData
{
public:
	/// コンストラクタ
	ModelData(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);

	/// デストラクタ
	~ModelData();

	/// 頂点バッファを取得する
	std::shared_ptr<VertexBuffer> GetVertexBuffer();

	/// インデックスバッファを取得する
	std::shared_ptr<IndexBuffer> GetIndexBuffer();

protected:
	std::shared_ptr<VertexBuffer> mVertexBuffer;
	std::shared_ptr<IndexBuffer> mIndexBuffer;
	std::shared_ptr<DescriptorHeap> mDescHeap;
private:
};

