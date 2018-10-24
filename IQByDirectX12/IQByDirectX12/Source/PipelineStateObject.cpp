#include "PipelineStateObject.h"
#include "Device.h"


PipelineStateObject::PipelineStateObject(std::shared_ptr<Device> device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC & pipelineStateDesc)
{
	(*device)->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&mPipelineStateObject));
}


PipelineStateObject::~PipelineStateObject()
{
}

std::shared_ptr<PipelineStateObject> PipelineStateObject::Create(std::shared_ptr<Device> device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC & pipelineStateDesc)
{
	auto pso = std::shared_ptr<PipelineStateObject>(new PipelineStateObject(device, pipelineStateDesc));
	if (!pso->mPipelineStateObject)
	{
		pso = std::shared_ptr<PipelineStateObject>();
	}
	return pso;
}

ComPtr<ID3D12PipelineState> PipelineStateObject::GetPipelineStateObject() const
{
	return mPipelineStateObject;
}
