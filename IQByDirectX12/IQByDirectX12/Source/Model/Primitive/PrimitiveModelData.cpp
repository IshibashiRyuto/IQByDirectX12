#include "PrimitiveModelData.h"
#include "../Dx12/ConstantBuffer.h"
#include "../Dx12/PipelineStateObject.h"
#include "../Dx12/Buffer/InstanceDataBuffer.h"
#include "../InstancingDataManager.h"
#include"../Dx12/RootSignature.h"
#include"../Dx12/GraphicsCommandList.h"


const int MATERIAL_ROOT_INDEX = 3;


PrimitiveModelData::PrimitiveModelData(std::shared_ptr<Device> device,
	const std::vector<Primitive::Vertex>& vertices,
	const std::vector<Primitive::Index>& indexes,
	const Primitive::Material & material,
	std::shared_ptr<PipelineStateObject> pipelineStateObject,
	std::shared_ptr<PipelineStateObject> shadowPSO,
	std::shared_ptr<RootSignature> rootSignature)
	: ModelData
	(
		VertexBuffer::Create(device, vertices.data(), sizeof(Primitive::Vertex), vertices.size(), L"Primitive"),
		IndexBuffer::Create(device, indexes.data(), sizeof(short), indexes.size(), L"PrimitiveIndex"),
		DescriptorHeap::Create(device, 1),
		pipelineStateObject,
		shadowPSO,
		rootSignature
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

void PrimitiveModelData::Draw(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData & instanceData) const
{
	(*commandList)->SetPipelineState(mPipelineStateObject->GetPipelineStateObject().Get());

	mDescHeap->SetBindHeapIndex(0);
	mRootSignature->SetBindDescriptorHeap(MATERIAL_ROOT_INDEX, mDescHeap);

	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	(*commandList)->IASetVertexBuffers(0, 2, vbViews);
	(*commandList)->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	mRootSignature->SetRootSignature(commandList);
	mRootSignature->SetRootParameter(commandList);

	(*commandList)->DrawIndexedInstanced(mIndexBuffer->GetIndexCount(), instanceData.nowInstanceCount, 0, 0, 0);
}

void PrimitiveModelData::DrawShadow(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData & instanceData) const
{
	(*commandList)->SetPipelineState(mShadowPSO->GetPipelineStateObject().Get());

	mDescHeap->SetBindHeapIndex(0);
	mRootSignature->SetBindDescriptorHeap(MATERIAL_ROOT_INDEX, mDescHeap);

	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	(*commandList)->IASetVertexBuffers(0, 2, vbViews);
	(*commandList)->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	mRootSignature->SetRootSignature(commandList);
	mRootSignature->SetRootParameter(commandList);

	(*commandList)->DrawIndexedInstanced(mIndexBuffer->GetIndexCount(), instanceData.nowInstanceCount, 0, 0, 0);
}
