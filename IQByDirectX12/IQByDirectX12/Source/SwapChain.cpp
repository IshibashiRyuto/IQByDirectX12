#include <iostream>
#include "SwapChain.h"
#include "CommandQueue.h"
#include "Debug\DebugLayer.h"


SwapChain::SwapChain()
{
}


SwapChain::~SwapChain()
{
}

std::shared_ptr<SwapChain> SwapChain::Create(std::shared_ptr<CommandQueue> commandQueue, const Window & window, unsigned int renderTargetNum)
{
	auto instance = std::shared_ptr<SwapChain>(new SwapChain());

	// ファクトリ生成
	if (!instance->CreateFactory())
	{
		return nullptr;
	}

	// スワップチェイン生成
	if (!instance->CreateSwapChain(commandQueue, window, renderTargetNum))
	{
		return nullptr;
	}
	return instance;
}

void SwapChain::Swap()
{
	mSwapChain->Present(1, 0);
}

unsigned int SwapChain::GetBackBufferIndex()
{
	return mSwapChain->GetCurrentBackBufferIndex();
}

ComPtr<IDXGISwapChain1> SwapChain::GetSwapChain() const
{
	return mSwapChain;
}

bool SwapChain::CreateFactory()
{
	auto result = CreateDXGIFactory1(IID_PPV_ARGS(&mFactory));
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create DXGIFactory.");
		return false;
	}
	return true;
}

bool SwapChain::CreateSwapChain(std::shared_ptr<CommandQueue> commandQueue, const Window & window, unsigned int renderTargetNum)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	auto windowRect = window.GetWindowRect();

	swapChainDesc.Width = windowRect.right - windowRect.left;
	swapChainDesc.Height = windowRect.bottom - windowRect.top;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = renderTargetNum;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = 0;

	auto result = mFactory->CreateSwapChainForHwnd(commandQueue->GetCommandQueue().Get(),
		window.GetWindowHandle(),
		&swapChainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)mSwapChain.GetAddressOf());
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create SwapChain.");
	}

	return true;
}
