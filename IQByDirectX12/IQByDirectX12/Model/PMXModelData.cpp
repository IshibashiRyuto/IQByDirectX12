#include "PMXModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"

PMXModelData::PMXModelData(ComPtr<ID3D12Device> device, std::vector<PMX::Vertex> vertexData, std::vector<PMX::Index> indexData)
	: ModelData(VertexBuffer::Create(device, vertexData.data(), vertexData.size(), sizeof(PMX::Vertex)), 
		IndexBuffer::Create(device, indexData.data(), indexData.size(), sizeof(PMX::Index)),
		DescriptorHeap::Create(device, 2))
{
}

PMXModelData::~PMXModelData()
{
}

std::shared_ptr<PMXModelData> PMXModelData::Create(ComPtr<ID3D12Device> device, std::vector<PMX::Vertex> vertexData, std::vector<PMX::Index> indexData)
{
	return std::shared_ptr<PMXModelData>(new PMXModelData(device, vertexData, indexData));
}

std::shared_ptr<PMXModelData> PMXModelData::Create(ComPtr<ID3D12Device> device, const PMX::ModelDataDesc & modelDataDesc)
{
	return std::shared_ptr<PMXModelData>(new PMXModelData(device, modelDataDesc.vertices, modelDataDesc.indexies));
}
