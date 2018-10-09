#include "PMDModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"
#include "../InstanceBuffer.h"
#include "InstancingDataManager.h"
#include "../Device.h"

PMDModelData::PMDModelData(std::shared_ptr<Device> device, const std::vector<PMDVertex>& vertexData, const std::vector<unsigned short>& indexData, const std::vector<PMDMaterial>& materials)
	: ModelData(VertexBuffer::Create(device, (void*)vertexData.data(), vertexData.size(), sizeof(PMDVertex)), IndexBuffer::Create(device->GetDevice(), (void*)indexData.data(), indexData.size(), sizeof(short)), DescriptorHeap::Create(device->GetDevice(), 1 + (int)materials.size()) )
{
	SetVertexData(vertexData);
	SetIndexData(indexData);
	SetMaterialData(device,materials);
}

PMDModelData::~PMDModelData()
{
}

std::shared_ptr<PMDModelData> PMDModelData::Create(std::shared_ptr<Device> device, const std::vector<PMDVertex>& vertexData, const std::vector<unsigned short>& indexData, const std::vector<PMDMaterial>& materials)
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

void PMDModelData::SetMaterialData(std::shared_ptr<Device> device, const std::vector<PMDMaterial>& materials)
{
	mMaterialCount = (unsigned int)materials.size();
	mMaterials = materials;
	mMaterialData = ConstantBuffer::Create(device, sizeof(PMDMaterialData), mMaterialCount);
	for (unsigned int i = 0; i < mMaterialCount; ++i)
	{
		PMDMaterialData data;
		data.alpha = materials[i].alpha;
		data.diffuseColor = materials[i].diffuseColor;
		data.specularColor = materials[i].specularColor;
		data.specularity = materials[i].specularity;
		data.ambientColor = materials[i].ambientColor;
		data.isUseTexture = (strcmp(materials[i].textureFileName, "") == 0) ? 0 : 1;
		mMaterialData->SetData(&data, sizeof(PMDMaterialData), i);
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
