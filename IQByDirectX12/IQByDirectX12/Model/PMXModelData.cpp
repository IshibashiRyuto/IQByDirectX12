#include "PMXModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"
#include "InstancingDataManager.h"
#include "../InstanceBuffer.h"

PMXModelData::PMXModelData(ComPtr<ID3D12Device> device, std::vector<PMX::Vertex> vertexData, std::vector<PMX::Index> indexData,int materialCount)
	: ModelData(VertexBuffer::Create(device, vertexData.data(), vertexData.size(), sizeof(PMX::Vertex)), 
		IndexBuffer::Create(device, indexData.data(), indexData.size(), sizeof(PMX::Index)),
		DescriptorHeap::Create(device, 1 + materialCount))
	, mMaterialDataBuffer( ConstantBuffer::Create(device, sizeof(PMX::Material), materialCount) )
{
}

PMXModelData::~PMXModelData()
{
}

std::shared_ptr<PMXModelData> PMXModelData::Create(ComPtr<ID3D12Device> device, std::vector<PMX::Vertex> vertexData, std::vector<PMX::Index> indexData)
{
	return std::shared_ptr<PMXModelData>(new PMXModelData(device, vertexData, indexData, 1));
}

std::shared_ptr<PMXModelData> PMXModelData::Create(ComPtr<ID3D12Device> device, const PMX::ModelDataDesc & modelDataDesc)
{
	auto modelData = std::shared_ptr<PMXModelData>(new PMXModelData(device, modelDataDesc.vertices, modelDataDesc.indexies, modelDataDesc.materials.size()));
	modelData->SetMaterial(modelDataDesc.materials);
	return modelData;
}

void PMXModelData::Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData & instanceData) const
{
	mDescHeap->BindGraphicsCommandList(graphicsCommandList);
	mDescHeap->BindRootDescriptorTable(0, 0);
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	graphicsCommandList->IASetVertexBuffers(0, 2, vbViews);
	graphicsCommandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	int indexOffset = 0;
	for (int i = 0; i < mMaterialData.size(); ++i)
	{
		mDescHeap->BindRootDescriptorTable(1, i + 1);
		graphicsCommandList->DrawIndexedInstanced(mMaterialData[i].vertsNum, instanceData.nowInstanceCount, indexOffset, 0, 0);
		indexOffset += mMaterialData[i].vertsNum;
	}
}

void PMXModelData::SetMaterial(const std::vector<PMX::Material>& materials)
{
	mMaterialData.resize(materials.size());
	for (int i = 0; i < materials.size(); ++i)
	{
		mMaterialData[i].diffuse = materials[i].diffuse;
		mMaterialData[i].specular = materials[i].specular;
		mMaterialData[i].specularity = materials[i].specularFactor;
		mMaterialData[i].ambient = materials[i].ambient;
		mMaterialData[i].vertsNum = materials[i].vertNum;
		mMaterialDataBuffer->SetData(&mMaterialData[i], sizeof(PMX::MaterialData), i);
		mDescHeap->SetConstantBufferView(mMaterialDataBuffer->GetConstantBufferView(i), i + 1);
	}
}
