#include <iostream>
#include <d3dx12.h>
#include "Device.h"
#include "RenderTarget.h"
#include "Debug\DebugLayer.h"
#include "Texture/RenderTargetTexture.h"



RenderTarget::RenderTarget(std::shared_ptr<Device> device)
	: RENDER_TARGET_VIEW_DESCRIPTOR_SIZE(device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
	, mRenderTargetIndex(0)
{
}


RenderTarget::~RenderTarget()
{
}

std::shared_ptr<RenderTarget> RenderTarget::Create(std::shared_ptr<Device> device, ComPtr<IDXGISwapChain1> swapChain, int renderTargetsNum)
{
	auto renderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(device));

	// ディスクリプタパラメータ定義
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.NumDescriptors = renderTargetsNum;
	
	// RTVディスクリプタヒープ生成
	{
		auto result = device->GetDevice()->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&(renderTarget->mRTVDescHeap)));
		if (FAILED(result))
		{
			DebugLayer::GetInstance().PrintDebugMessage("Failed Create RTV Descriptor Heap.");
			return nullptr;
		}
	}

	// レンダーターゲットビューの作成
	renderTarget->mRenderTargets.resize(renderTargetsNum);


	CD3DX12_CPU_DESCRIPTOR_HANDLE descHandle(renderTarget->mRTVDescHeap->GetCPUDescriptorHandleForHeapStart());
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChain->GetDesc(&swapChainDesc);
	int backBufferNum = swapChainDesc.BufferCount;
	for (int i = 0; i < renderTargetsNum; ++i)
	{
		if (i < backBufferNum)
		{

			ComPtr<ID3D12Resource> resource;
			auto result = swapChain->GetBuffer(i, IID_PPV_ARGS(&resource));
			if (FAILED(result))
			{
				DebugLayer::GetInstance().PrintDebugMessage("Failed Create Render Target.");
				return nullptr;
			}

			renderTarget->mRenderTargets[i] = RenderTargetTexture::Create(resource);
		}
		else
		{
			renderTarget->mRenderTargets[i] = RenderTargetTexture::Create(device, 1280, 720);
		}

		auto rtvDesc = renderTarget->mRenderTargets[i]->GetRenderTargetViewDesc();
		
		device->GetDevice()->CreateRenderTargetView(renderTarget->mRenderTargets[i]->GetTextureData().Get(), &rtvDesc, descHandle);
		descHandle.Offset(1, renderTarget->RENDER_TARGET_VIEW_DESCRIPTOR_SIZE);
	}

	return renderTarget;
}

void RenderTarget::ChangeRenderTarget(ComPtr<ID3D12GraphicsCommandList> commandList, int targetIndex)
{
	mRenderTargetIndex = targetIndex;
	commandList->ResourceBarrier(1, &mRenderTargets[targetIndex]->GetTransitionBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET));
}

void RenderTarget::FinishRendering(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	commandList->ResourceBarrier(1, &mRenderTargets[mRenderTargetIndex]->GetTransitionBarrier(D3D12_RESOURCE_STATE_PRESENT));
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetRTVHandle()
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(mRTVDescHeap->GetCPUDescriptorHandleForHeapStart(), mRenderTargetIndex, RENDER_TARGET_VIEW_DESCRIPTOR_SIZE);
}

void RenderTarget::ClearRenderTarget(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	const float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	commandList->ClearRenderTargetView(GetRTVHandle(), color, 0, nullptr);
}

std::shared_ptr<RenderTargetTexture> RenderTarget::GetRenderTargetTexture(int targetIndex)
{
	return mRenderTargets[targetIndex];
}
