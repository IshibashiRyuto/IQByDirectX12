#include "PipelineStateObject.h"
#include "Device.h"
#include "RenderState.h"

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

std::shared_ptr<PipelineStateObject> PipelineStateObject::Create(std::shared_ptr<Device> device, const std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout, const RenderState & renderState, const ShaderList & shaderList)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = renderState.GetPipelineStateDesc();



	return std::shared_ptr<PipelineStateObject>();
}

ComPtr<ID3D12PipelineState> PipelineStateObject::GetPipelineStateObject() const
{
	return mPipelineStateObject;
}
