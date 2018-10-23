#include "ModelData.h"
#include "InstancingDataManager.h"
#include "../InstanceBuffer.h"

ModelData::ModelData(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<DescriptorHeap> descriptorHeap)
	: mVertexBuffer(vertexBuffer)
	, mIndexBuffer(indexBuffer)
	, mDescHeap(descriptorHeap)
{
}

ModelData::~ModelData()
{
}

std::shared_ptr<DescriptorHeap> ModelData::GetDescriptorHeap() const
{
	return mDescHeap;
}

std::shared_ptr<Pose> ModelData::_DebugGetPose() const
{
	return mPose;
}

void ModelData::Update()
{
}

void ModelData::Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instancingData) const
{
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instancingData.instanceBuffer->GetVertexBufferView() };
	graphicsCommandList->IASetVertexBuffers(0, 2, vbViews);
	graphicsCommandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	graphicsCommandList->DrawIndexedInstanced(mIndexBuffer->GetIndexCount(), instancingData.nowInstanceCount, 0, 0, 0);
}

std::shared_ptr<VertexBuffer> ModelData::GetVertexBuffer() const
{
	return mVertexBuffer;
}

std::shared_ptr<IndexBuffer> ModelData::GetIndexBuffer() const
{
	return mIndexBuffer;
}

