#include "ModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"

ModelData::ModelData(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer)
	: mVertexBuffer(vertexBuffer)
	, mIndexBuffer(indexBuffer)
{
}

ModelData::~ModelData()
{
}

std::shared_ptr<VertexBuffer> ModelData::GetVertexBuffer()
{
	return mVertexBuffer;
}

std::shared_ptr<IndexBuffer> ModelData::GetIndexBuffer()
{
	return mIndexBuffer;
}

