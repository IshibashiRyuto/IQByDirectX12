#include <iostream>
#include <d3dx12.h>
#include "RootSignature.h"
#include "../Debug/DebugLayer.h"
#include "GraphicsCommandList.h"
#include "DescriptorHeap.h"


RootSignature::RootSignature()
{
	// 静的サンプラの設定
	{
		mStaticSamplerDesc = D3D12_STATIC_SAMPLER_DESC();
		mStaticSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		mStaticSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		mStaticSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		mStaticSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		mStaticSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		mStaticSamplerDesc.MinLOD = 0.0f;
		mStaticSamplerDesc.MipLODBias = 0.0f;
		mStaticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		mStaticSamplerDesc.ShaderRegister = 0;
		mStaticSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		mStaticSamplerDesc.RegisterSpace = 0;
		mStaticSamplerDesc.MaxAnisotropy = 0;
		mStaticSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS;
	}
}


RootSignature::~RootSignature()
{
}

std::shared_ptr<RootSignature> RootSignature::Create()
{
	return std::shared_ptr<RootSignature>(new RootSignature());
}

bool RootSignature::ConstructRootSignature(ComPtr<ID3D12Device> device)
{
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	
	// ルートパラメータの設定
	for (int i = 0; i < mRootParameters.size(); ++i)
	{
		// parameterType : DescriptorTableの場合
		mRootParameters[i].DescriptorTable.NumDescriptorRanges = (UINT)mDescriptorRanges[i].size();
		mRootParameters[i].DescriptorTable.pDescriptorRanges = mDescriptorRanges[i].data();
	}

	// シグネチャ作成
	{
		auto rootSignatureDesc = CD3DX12_ROOT_SIGNATURE_DESC((UINT)mRootParameters.size(), mRootParameters.data(), 1, &mStaticSamplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

		if (FAILED(result))
		{
			DebugLayer::GetInstance().PrintDebugMessage("Failed Create Signature.");
			return false;
		}
	}

	// ルートシグネチャ作成
	{
		auto result = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
		if (FAILED(result))
		{
			DebugLayer::GetInstance().PrintDebugMessage("Failed Create Root Signature.");
			return false;
		}
	}

	// デスクリプタヒープ格納用の場所確保
	mDescriptorHeap.clear();
	mDescriptorHeap.resize(mRootParameters.size());

	return true;
}

void RootSignature::AddDescriptorRange(int rootParamIndex, const D3D12_DESCRIPTOR_RANGE & descriptorRange)
{
	if (mRootParameters.size() <= rootParamIndex)
	{
		return;
	}
	mDescriptorRanges[rootParamIndex].push_back(descriptorRange);
}

void RootSignature::AddDescriptorRange(int rootParamIndex, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT numDescriptors, UINT baseShaderRegister)
{
	D3D12_DESCRIPTOR_RANGE descRange;
	descRange.RangeType = rangeType;
	descRange.NumDescriptors = numDescriptors;
	descRange.RegisterSpace = 0;
	descRange.BaseShaderRegister = baseShaderRegister;
	descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	AddDescriptorRange(rootParamIndex, descRange);
}

int RootSignature::AddRootParameter(D3D12_SHADER_VISIBILITY shaderVisibility, D3D12_ROOT_PARAMETER_TYPE paramType)
{
	D3D12_ROOT_PARAMETER rootParam;
	rootParam.ParameterType = paramType;
	rootParam.ShaderVisibility = shaderVisibility;
	mRootParameters.push_back(rootParam);
	return (int)(mRootParameters.size() - 1);
}

ComPtr<ID3D12RootSignature> RootSignature::GetRootSignature() const
{
	return mRootSignature;
}

void RootSignature::SetBindDescriptorHeap(unsigned int rootParamIndex, std::shared_ptr<DescriptorHeap> descHeap)
{
	if (static_cast<size_t>(rootParamIndex) >= mDescriptorHeap.size())
	{
		return;
	}
	mDescriptorHeap[rootParamIndex] = descHeap;
}

void RootSignature::SetRootParameter(std::shared_ptr<GraphicsCommandList> commandList) const
{
	for (unsigned int i = 0; i < mDescriptorHeap.size(); ++i)
	{
		mDescriptorHeap[i]->BindRootParameter(commandList, i);
	}
}

void RootSignature::SetRootSignature(std::shared_ptr<GraphicsCommandList> commandList) const
{
	(*commandList)->SetGraphicsRootSignature(mRootSignature.Get());
}
