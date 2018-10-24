/*
	@file	ModelData.h
	@brief	モデルデータを格納するクラス
	@param	history
	2018/08/28	初版作成
*/
#pragma once
// システムヘッダインクルード
#include<memory>
#include <list>

//自作ヘッダインクルード
#include "../Math/Math.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"

// クラス使用宣言
struct InstanceData;
class Pose;
class PipelineStateObject;

class ModelData
{
public:
	/// コンストラクタ
	ModelData(std::shared_ptr<VertexBuffer> vertexBuffer, 
		std::shared_ptr<IndexBuffer> indexBuffer,
		std::shared_ptr<DescriptorHeap> descriptorHeap,
		std::shared_ptr<PipelineStateObject> pipelineStateObject);

	/// デストラクタ
	virtual ~ModelData();

	/// 頂点バッファを取得する
	std::shared_ptr<VertexBuffer> GetVertexBuffer() const;

	/// インデックスバッファを取得する
	std::shared_ptr<IndexBuffer> GetIndexBuffer() const;

	/// デスクリプタヒープを取得する
	std::shared_ptr<DescriptorHeap> GetDescriptorHeap() const;

	/// Debug
	/// 姿勢情報を取得する
	std::shared_ptr<Pose> _DebugGetPose() const;

	/// 更新処理
	virtual void Update();

	/// 描画処理
	virtual void Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;

protected:
	std::shared_ptr<VertexBuffer>			mVertexBuffer;
	std::shared_ptr<IndexBuffer>			mIndexBuffer;
	std::shared_ptr<DescriptorHeap>			mDescHeap;
	std::shared_ptr<Pose>					mPose;
	std::shared_ptr<PipelineStateObject>	mPipelineStateObject;
private:
};

