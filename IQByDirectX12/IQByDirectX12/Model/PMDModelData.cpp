#include "PMDModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"

PMDModelData::PMDModelData(ComPtr<ID3D12Device> device, const std::vector<PMDVertex>& vertexData, const std::vector<unsigned short>& indexData, const std::vector<PMDMaterial>& materials)
	: ModelData(VertexBuffer::Create(device, (void*)vertexData.data(), vertexData.size(), sizeof(PMDVertex)), IndexBuffer::Create(device, (void*)indexData.data(), indexData.size(), sizeof(short)), DescriptorHeap::Create(device, 2) )
{
	SetVertexData(vertexData);
	SetIndexData(indexData);
	SetMaterialData(materials);
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

void PMDModelData::SetMaterialData(const std::vector<PMDMaterial>& materials)
{
	mMaterialCount = (unsigned int)materials.size();
	mMaterials = materials;
}
