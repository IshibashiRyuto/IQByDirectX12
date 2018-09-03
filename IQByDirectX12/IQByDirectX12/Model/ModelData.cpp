#include "ModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"

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

std::shared_ptr<VertexBuffer> ModelData::GetVertexBuffer() const
{
	return mVertexBuffer;
}

std::shared_ptr<IndexBuffer> ModelData::GetIndexBuffer() const
{
	return mIndexBuffer;
}

