#include <iostream>
#include <d3dx12.h>
#include "DepthBuffer.h"



DepthBuffer::DepthBuffer(const D3D12_CLEAR_VALUE& depthClearValue)
	: DEPTH_CLEAR_VALUE(depthClearValue)
{
}


DepthBuffer::~DepthBuffer()
{
}

std::shared_ptr<DepthBuffer> DepthBuffer::Create(ComPtr<ID3D12Device> device, int windowWidth, int windowHeight)
{
	// クリア値を設定し、DepthBufferクラスを生成
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	auto depthBuffer = std::shared_ptr<DepthBuffer>(new DepthBuffer(depthClearValue));

	// 深度バッファの作成
	D3D12_RESOURCE_DESC depthBufferDesc{};
	depthBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthBufferDesc.Width = (UINT)windowWidth;
	depthBufferDesc.Height = (UINT)windowHeight;
	depthBufferDesc.DepthOrArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;


	{
		auto result = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&depthBufferDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depthBuffer->DEPTH_CLEAR_VALUE,
			IID_PPV_ARGS(&(depthBuffer->mDepthBuffer)));
		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create Depth Buffer." << std::endl;
#endif
			return nullptr;
		}
	}

	// デプスステンシルビューの作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	{
		auto result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&(depthBuffer->mDSVDescHeap)));
		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create Depth Stencil View Descriptor Heap." << std::endl;
#endif
			return nullptr;
		}
	}
	depthBuffer->mDSVCPUHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(depthBuffer->mDSVDescHeap->GetCPUDescriptorHandleForHeapStart());

	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	device->CreateDepthStencilView(depthBuffer->mDepthBuffer.Get(), &dsvDesc, depthBuffer->mDSVCPUHandle);

	return depthBuffer;
}

D3D12_CPU_DESCRIPTOR_HANDLE DepthBuffer::GetDSVHandle()
{
	return mDSVCPUHandle;
}

void DepthBuffer::ClearDepthBuffer(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	commandList->ClearDepthStencilView(GetDSVHandle(), D3D12_CLEAR_FLAG_DEPTH, DEPTH_CLEAR_VALUE.DepthStencil.Depth, 0, 0, nullptr);
}
