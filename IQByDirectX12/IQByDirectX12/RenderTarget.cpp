#include <iostream>
#include <d3dx12.h>
#include "RenderTarget.h"



RenderTarget::RenderTarget(ComPtr<ID3D12Device> device)
	: RENDER_TARGET_VIEW_DESCRIPTOR_SIZE(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
	, mRenderTargetIndex(0)
{
}


RenderTarget::~RenderTarget()
{
}

std::shared_ptr<RenderTarget> RenderTarget::Create(ComPtr<ID3D12Device> device, ComPtr<IDXGISwapChain1> swapChain, int renderTargetsNum)
{
	auto renderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(device));

	// ディスクリプタパラメータ定義
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.NumDescriptors = renderTargetsNum;
	
	// RTVディスクリプタヒープ生成
	{
		auto result = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&(renderTarget->mRTVDescHeap)));
		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create RTV Descriptor Heap." << std::endl;
#endif
			return nullptr;
		}
	}

	// レンダーターゲットビューの作成
	renderTarget->mRenderTargets.resize(renderTargetsNum);

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE descHandle(renderTarget->mRTVDescHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < renderTargetsNum; ++i)
	{
		auto result = swapChain->GetBuffer(i, IID_PPV_ARGS(&(renderTarget->mRenderTargets[i])));
		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create Render Target." << std::endl;
#endif
			return nullptr;
		}
		device->CreateRenderTargetView(renderTarget->mRenderTargets[i].Get(), &rtvDesc, descHandle);
		descHandle.Offset(1, renderTarget->RENDER_TARGET_VIEW_DESCRIPTOR_SIZE);
	}

	return renderTarget;
}

void RenderTarget::ChangeRenderTarget(ComPtr<ID3D12GraphicsCommandList> commandList, int targetIndex)
{
	mRenderTargetIndex = targetIndex;
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[targetIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
}

void RenderTarget::FinishRendering(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mRenderTargetIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetRTVHandle()
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(mRTVDescHeap->GetCPUDescriptorHandleForHeapStart(), mRenderTargetIndex, RENDER_TARGET_VIEW_DESCRIPTOR_SIZE);
}

void RenderTarget::ClearRenderTarget(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	float color[4] = { 0.0f,0.0f,0.0f,1.0f };
	commandList->ClearRenderTargetView(GetRTVHandle(), color, 0, nullptr);
}
