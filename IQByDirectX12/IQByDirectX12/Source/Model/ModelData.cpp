#include "ModelData.h"
#include "../InstancingDataManager.h"
#include "../Dx12/Buffer/InstanceDataBuffer.h"
#include "../Dx12/PipelineStateObject.h"
#include "../Dx12/GraphicsCommandList.h"

ModelData::ModelData(std::shared_ptr<VertexBuffer> vertexBuffer,
	std::shared_ptr<IndexBuffer> indexBuffer, 
	std::shared_ptr<DescriptorHeap> descriptorHeap,
	std::shared_ptr<PipelineStateObject> pipelineStateObject,
	std::shared_ptr<PipelineStateObject> shadowPSO,
	std::shared_ptr<RootSignature> rootSignature)
	: mVertexBuffer(vertexBuffer)
	, mIndexBuffer(indexBuffer)
	, mDescHeap(descriptorHeap)
	, mPipelineStateObject(pipelineStateObject)
	, mShadowPSO(shadowPSO)
	, mRootSignature(rootSignature)
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

void ModelData::Draw(std::shared_ptr<GraphicsCommandList> graphicsCommandList, const InstanceData& instancingData) const
{
	(*graphicsCommandList)->SetPipelineState(mPipelineStateObject->GetPipelineStateObject().Get());

	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instancingData.instanceBuffer->GetVertexBufferView() };
	(*graphicsCommandList)->IASetVertexBuffers(0, 2, vbViews);
	(*graphicsCommandList)->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	(*graphicsCommandList)->DrawIndexedInstanced(mIndexBuffer->GetIndexCount(), instancingData.nowInstanceCount, 0, 0, 0);
}

void ModelData::DrawShadow(std::shared_ptr<GraphicsCommandList> graphicsCommandList, const InstanceData & instanceData) const
{
	(*graphicsCommandList)->SetPipelineState(mShadowPSO->GetPipelineStateObject().Get());

	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	(*graphicsCommandList)->IASetVertexBuffers(0, 2, vbViews);
	(*graphicsCommandList)->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	(*graphicsCommandList)->DrawIndexedInstanced(mIndexBuffer->GetIndexCount(), instanceData.nowInstanceCount, 0, 0, 0);
}

std::shared_ptr<VertexBuffer> ModelData::GetVertexBuffer() const
{
	return mVertexBuffer;
}

std::shared_ptr<IndexBuffer> ModelData::GetIndexBuffer() const
{
	return mIndexBuffer;
}

