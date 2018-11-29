/*
	@file	ModelData.h
	@brief	モデルデータを格納するクラス
	@author	Ishibashi Ryuto
	@date	2018/08/28	初版作成
			2018/11/25	バッファクラスのリファクタリングに対応
			2018/11/27	DrawNoMat追加
*/
#pragma once
/* ヘッダインクルード */
#include<memory>
#include <list>
#include "../Math/Math.h"
#include "../Dx12/Buffer/VertexBuffer.h"
#include "../Dx12/Buffer/IndexBuffer.h"
#include "../Dx12/DescriptorHeap.h"

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

	/**
	*	@brief	マテリアルを使用せずに描画する(モデルのガワだけ描画)
	*/
	virtual void DrawNoMat(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;

protected:
	std::shared_ptr<VertexBuffer>			mVertexBuffer;
	std::shared_ptr<IndexBuffer>			mIndexBuffer;
	std::shared_ptr<DescriptorHeap>			mDescHeap;
	std::shared_ptr<Pose>					mPose;
	std::shared_ptr<PipelineStateObject>	mPipelineStateObject;
private:
};

