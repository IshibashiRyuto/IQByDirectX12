#include <iostream>
#include <d3dx12.h>
#include "RootSignature.h"


RootSignature::RootSignature()
{
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
		mRootParameters[i].DescriptorTable.NumDescriptorRanges = mDescriptorRanges[i].size();
		mRootParameters[i].DescriptorTable.pDescriptorRanges = mDescriptorRanges[i].data();
	}

	// シグネチャ作成
	{
		auto rootSignatureDesc = CD3DX12_ROOT_SIGNATURE_DESC(mRootParameters.size(), mRootParameters.data(), 1, &mStaticSamplerDesc);

		auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create Signature." << std::endl;
#endif
			return false;
		}
	}

	// ルートシグネチャ作成
	{
		auto result = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create Root Signature." << std::endl;
#endif
			return false;
		}
	}

	return true;
}

void RootSignature::AddDescriptorRange(int rootParamIndex, const D3D12_DESCRIPTOR_RANGE & descriptorRange)
{
}

void RootSignature::AddDescriptorRange(int rootParamIndex, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT descriptorNum, UINT baseShaderRegister)
{
}

int RootSignature::AddRootParameter(D3D12_SHADER_VISIBILITY shaderVisibility, D3D12_ROOT_PARAMETER_TYPE paramType)
{
	return 0;
}

ComPtr<ID3D12RootSignature> RootSignature::GetRootSignature() const
{
	return mRootSignature;
}
