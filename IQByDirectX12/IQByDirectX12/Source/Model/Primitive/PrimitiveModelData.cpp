#include "PrimitiveModelData.h"
#include "../Dx12/ConstantBuffer.h"
#include "../Dx12/PipelineStateObject.h"
#include "../Dx12/Buffer/InstanceDataBuffer.h"
#include "../InstancingDataManager.h"

const int MATERIAL_ROOT_INDEX = 1;


PrimitiveModelData::PrimitiveModelData(std::shared_ptr<Device> device, const std::vector<Primitive::Vertex>& vertices, const std::vector<Primitive::Index>& indexes, const Primitive::Material & material, std::shared_ptr<PipelineStateObject> pipelineStateObject)
	: ModelData
	(
		VertexBuffer::Create(device, vertices.data(), sizeof(Primitive::Vertex), vertices.size(), L"Primitive"),
		IndexBuffer::Create(device, indexes.data(), sizeof(short), indexes.size(), L"PrimitiveIndex"),
		DescriptorHeap::Create(device, 1),
		pipelineStateObject
	)
	, mMaterial(material)
{
	mMaterialBuf = ConstantBuffer::Create(device, sizeof(Primitive::Material));
	mMaterialBuf->SetData(&mMaterial, sizeof(Primitive::Material));
	mDescHeap->SetConstantBufferView(mMaterialBuf->GetConstantBufferView(), 0);
}

PrimitiveModelData::~PrimitiveModelData()
{
}

void PrimitiveModelData::Update()
{
}

void PrimitiveModelData::Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData & instanceData) const
{
	graphicsCommandList->SetPipelineState(mPipelineStateObject->GetPipelineStateObject().Get());

	mDescHeap->BindGraphicsCommandList(graphicsCommandList);
	mDescHeap->BindRootDescriptorTable(MATERIAL_ROOT_INDEX, 0);

	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	graphicsCommandList->IASetVertexBuffers(0, 2, vbViews);
	graphicsCommandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	graphicsCommandList->DrawIndexedInstanced(mIndexBuffer->GetIndexCount(), instanceData.nowInstanceCount, 0, 0, 0);
}
