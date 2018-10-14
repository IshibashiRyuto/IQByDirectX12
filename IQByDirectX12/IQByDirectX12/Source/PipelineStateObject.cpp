#include "PipelineStateObject.h"



PipelineStateObject::PipelineStateObject()
{
}


PipelineStateObject::~PipelineStateObject()
{
}

ComPtr<ID3D12PipelineState> PipelineStateObject::GetPipelineStateObject() const
{
	return mPipelineStateObject;
}
