#include "PrimitiveModelData.h"



PrimitiveModelData::PrimitiveModelData(std::shared_ptr<VertexBuffer> vertexBuffer,
	std::shared_ptr<IndexBuffer> indexBuffer,
	std::shared_ptr<DescriptorHeap> descHeap,
	std::shared_ptr<PipelineStateObject> pipelineStateObject)
	: ModelData(vertexBuffer, indexBuffer, descHeap, pipelineStateObject)
{
}


PrimitiveModelData::~PrimitiveModelData()
{
}
