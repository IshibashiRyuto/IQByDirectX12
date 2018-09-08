#include "PMDModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"
#include "../InstanceBuffer.h"
#include "InstancingDataManager.h"

PMDModelData::PMDModelData(ComPtr<ID3D12Device> device, const std::vector<PMDVertex>& vertexData, const std::vector<unsigned short>& indexData, const std::vector<PMDMaterial>& materials)
	: ModelData(VertexBuffer::Create(device, (void*)vertexData.data(), vertexData.size(), sizeof(PMDVertex)), IndexBuffer::Create(device, (void*)indexData.data(), indexData.size(), sizeof(short)), DescriptorHeap::Create(device, 1 + (int)materials.size()) )
{
	SetVertexData(vertexData);
	SetIndexData(indexData);
	SetMaterialData(device,materials);
}

PMDModelData::~PMDModelData()
{
}

std::shared_ptr<PMDModelData> PMDModelData::Create(ComPtr<ID3D12Device> device, const std::vector<PMDVertex>& vertexData, const std::vector<unsigned short>& indexData, const std::vector<PMDMaterial>& materials)
{
	auto model = std::shared_ptr<PMDModelData>(new PMDModelData(device, vertexData, indexData, materials));
	if (model->mVertexBuffer == nullptr || model->mIndexBuffer == nullptr)
	{
		return nullptr;
	}
	return model;
}

void PMDModelData::SetVertexData(const std::vector<PMDVertex>& vertexData)
{
	mVertexCount = (unsigned int)vertexData.size();
	mVertex = vertexData;
}

void PMDModelData::SetIndexData(const std::vector<unsigned short>& indexData)
{
	mIndexCount = (unsigned int)indexData.size();
	mIndex = indexData;
}

void PMDModelData::SetMaterialData(ComPtr<ID3D12Device> device, const std::vector<PMDMaterial>& materials)
{
	mMaterialCount = (unsigned int)materials.size();
	mMaterials = materials;
	mMaterialData = ConstantBuffer::Create(device, sizeof(PMDMaterial), mMaterialCount);
	for (unsigned int i = 0; i < mMaterialCount; ++i)
	{
		mMaterialData->SetData(&mMaterials[i], sizeof(PMDMaterial), i);
		mDescHeap->SetConstantBufferView(mMaterialData->GetConstantBufferView(i), i + 1);
	}
}

void PMDModelData::Draw(ComPtr<ID3D12GraphicsCommandList> commandList, const InstanceData & instanceData) const
{
	mDescHeap->BindGraphicsCommandList(commandList);
	mDescHeap->BindRootDescriptorTable(0, 0);
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	commandList->IASetVertexBuffers(0, 2, vbViews);
	commandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	int indexOffset = 0;
	for (unsigned int i = 0; i < mMaterialCount; ++i)
	{
		mDescHeap->BindRootDescriptorTable(1, i + 1);
		commandList->DrawIndexedInstanced(mMaterials[i].faceVertexCount, instanceData.nowInstanceCount, indexOffset, 0, 0);
		indexOffset += mMaterials[i].faceVertexCount;
	}
}
