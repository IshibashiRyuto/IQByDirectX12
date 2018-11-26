#include "SpriteData.h"
#include "../InstancingDataManager.h"
#include "../Dx12/Buffer/InstanceDataBuffer.h"
#include "../Dx12/GraphicsCommandList.h"
#include <d3d12.h>
#include "../Dx12//PipelineStateObject.h"


SpriteData::SpriteData(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<DescriptorHeap> descHeap, std::shared_ptr<PipelineStateObject> pso)
	: mVertexBuffer(vertexBuffer)
	, mDescHeap(descHeap)
	, mPSO(pso)
{
}

SpriteData::~SpriteData()
{
}

std::shared_ptr<SpriteData> SpriteData::Create(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<DescriptorHeap> descHeap, std::shared_ptr<PipelineStateObject> pso)
{
	if (!vertexBuffer || !descHeap || !pso)
	{
		return nullptr;
	}
	return std::shared_ptr<SpriteData>(new SpriteData(vertexBuffer,descHeap,pso));
}

void SpriteData::Update()
{

}

void SpriteData::Draw(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData & instanceData) const
{
	(*commandList)->SetPipelineState(mPSO->GetPipelineStateObject().Get());

	mDescHeap->BindGraphicsCommandList(commandList->GetCommandList());
	mDescHeap->BindRootDescriptorTable(0, 0);


	D3D12_VERTEX_BUFFER_VIEW vertView[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	(*commandList)->IASetVertexBuffers(0, 2, vertView);
	(*commandList)->DrawInstanced(4, instanceData.nowInstanceCount, 0, 0);
}
