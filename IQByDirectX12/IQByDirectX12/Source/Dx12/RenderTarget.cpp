#include <iostream>
#include <d3dx12.h>
#include "Device.h"
#include "RenderTarget.h"
#include "../Debug/DebugLayer.h"
#include "../Dx12/Buffer/RenderTargetBuffer.h"
#include "../Dx12/Buffer/Texture.h"
#include "../Dx12/GraphicsCommandList.h"


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
	renderTarget->mRenderTextures.resize(renderTargetsNum);


	CD3DX12_CPU_DESCRIPTOR_HANDLE descHandle(renderTarget->mRTVDescHeap->GetCPUDescriptorHandleForHeapStart());
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChain->GetDesc(&swapChainDesc);
	int backBufferNum = swapChainDesc.BufferCount;
	int windowWidth = swapChainDesc.BufferDesc.Width;
	int windowHeight = swapChainDesc.BufferDesc.Height;
	auto bufferFormat = swapChainDesc.BufferDesc.Format;

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

			renderTarget->mRenderTargets[i] = RenderTargetBuffer::Create(resource,D3D12_RESOURCE_STATE_PRESENT);
		}
		else
		{
			renderTarget->mRenderTargets[i] = RenderTargetBuffer::Create(device, windowWidth, windowHeight);
		}

		renderTarget->mRenderTextures[i] = Texture::Create(renderTarget->mRenderTargets[i]);

		auto rtvDesc = renderTarget->mRenderTargets[i]->GetRenderTargetViewDesc();
		
		device->GetDevice()->CreateRenderTargetView(renderTarget->mRenderTargets[i]->GetResource().Get(), &rtvDesc, descHandle);
		descHandle.Offset(1, renderTarget->RENDER_TARGET_VIEW_DESCRIPTOR_SIZE);
	}

	return renderTarget;
}

void RenderTarget::ChangeRenderTarget(std::shared_ptr<GraphicsCommandList> commandList, int targetIndex)
{
	mRenderTargetIndex = targetIndex;
	mRenderTargets[targetIndex]->TransitionState(commandList, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

void RenderTarget::FinishRendering(std::shared_ptr<GraphicsCommandList> commandList)
{
	mRenderTargets[mRenderTargetIndex]->TransitionState(commandList, D3D12_RESOURCE_STATE_PRESENT);
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetRTVHandle()
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(mRTVDescHeap->GetCPUDescriptorHandleForHeapStart(), mRenderTargetIndex, RENDER_TARGET_VIEW_DESCRIPTOR_SIZE);
}

void RenderTarget::ClearRenderTarget(std::shared_ptr<GraphicsCommandList> commandList)
{
	auto clearColor = mRenderTargets[mRenderTargetIndex]->GetClearValue();
	(*commandList)->ClearRenderTargetView(GetRTVHandle(), clearColor.Color, 0, nullptr);
}

std::shared_ptr<RenderTargetBuffer> RenderTarget::GetRenderTargetBuffer(int targetIndex) const
{
	return mRenderTargets[targetIndex];
}

std::shared_ptr<Texture> RenderTarget::GetRenderTexture(int targetIndex) const
{
	return mRenderTextures[targetIndex];
}
