#include "PipelineStateObject.h"
#include "Device.h"
#include "RenderState.h"
#include "RootSignature.h"
#include "ShaderList.h"

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

std::shared_ptr<PipelineStateObject> PipelineStateObject::Create(
	std::shared_ptr<Device> device,
	const std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout,
	std::shared_ptr<RootSignature> rootSignature,
	const RenderState & renderState,
	const ShaderList & shaderList)
{
	//	レンダリングステートからパイプラインを構築して取得
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = renderState.GetPipelineStateDesc();

	//	ルートシグネチャ設定
	pipelineStateDesc.pRootSignature = rootSignature->GetRootSignature().Get();

	//	基本設定項目の設定
	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineStateDesc.NumRenderTargets = 1;
	pipelineStateDesc.SampleDesc.Count = 1;
	pipelineStateDesc.SampleDesc.Quality = 0;
	pipelineStateDesc.SampleMask = UINT_MAX;

	// 頂点レイアウトの設定
	pipelineStateDesc.InputLayout.NumElements = static_cast<UINT>(inputLayout.size());
	pipelineStateDesc.InputLayout.pInputElementDescs = inputLayout.data();

	// シェーダ設定
	if (shaderList.VS)
	{
		pipelineStateDesc.VS = shaderList.VS->GetShaderByteCode();
	}
	if (shaderList.HS)
	{
		pipelineStateDesc.HS = shaderList.HS->GetShaderByteCode();
	}
	if (shaderList.DS)
	{
		pipelineStateDesc.DS = shaderList.DS->GetShaderByteCode();
	}
	if (shaderList.GS)
	{
		pipelineStateDesc.GS = shaderList.GS->GetShaderByteCode();
	}
	if (shaderList.PS)
	{
		pipelineStateDesc.PS = shaderList.PS->GetShaderByteCode();
	}

	// パイプライン作成
	return Create(device, pipelineStateDesc);
}

ComPtr<ID3D12PipelineState> PipelineStateObject::GetPipelineStateObject() const
{
	return mPipelineStateObject;
}
