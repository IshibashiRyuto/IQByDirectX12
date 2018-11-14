#include "RenderTargetTexture.h"
#include "../Dx12/Device.h"
#include <d3dx12.h>

std::shared_ptr<RenderTargetTexture> RenderTargetTexture::Create(std::shared_ptr<Device> device, int width, int height, Math::Vector4 clearColor, DXGI_FORMAT format)
{
	// ÉäÉ\Å[ÉXÇÃçÏê¨
	ComPtr<ID3D12Resource> rsc;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC rscDesc = {};
	rscDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rscDesc.Alignment = 0;
	rscDesc.Width = width;
	rscDesc.Height = height;
	rscDesc.DepthOrArraySize = 1;
	rscDesc.MipLevels = 1;
	rscDesc.Format = format;
	rscDesc.SampleDesc.Count = 1;
	rscDesc.SampleDesc.Quality = 0;
	rscDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	rscDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	auto result = (*device)->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&rscDesc,
		D3D12_RESOURCE_STATE_PRESENT,
		&clearValue,
		IID_PPV_ARGS(&rsc));

	if (FAILED(result))
	{
		return nullptr;
	}

	return std::shared_ptr<RenderTargetTexture>(new RenderTargetTexture(rsc));
}

std::shared_ptr<RenderTargetTexture> RenderTargetTexture::Create(ComPtr<ID3D12Resource> buffer)
{
	return std::shared_ptr<RenderTargetTexture>(new RenderTargetTexture(buffer));
}

const D3D12_RENDER_TARGET_VIEW_DESC & RenderTargetTexture::GetRenderTargetViewDesc() const
{
	return mRTVDesc;
}

void RenderTargetTexture::SetRenderTargetViewDesc(const D3D12_RENDER_TARGET_VIEW_DESC & rtvDesc)
{
	mRTVDesc = rtvDesc;
}

D3D12_RESOURCE_BARRIER RenderTargetTexture::GetTransitionBarrier(D3D12_RESOURCE_STATES dstState)
{
	auto currentState = mCurrentResourceState;
	mCurrentResourceState = dstState;
	return CD3DX12_RESOURCE_BARRIER::Transition(mTextureData.Get(), currentState, dstState);
}

RenderTargetTexture::RenderTargetTexture(ComPtr<ID3D12Resource> textureData)
	: Texture(textureData)
{
	ConstructRTVDesc();
}

void RenderTargetTexture::ConstructRTVDesc()
{
	if (!mTextureData)
	{
		return;
	}

	auto rscDesc = mTextureData->GetDesc();
	
	mRTVDesc.Format = rscDesc.Format;
	mRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	mRTVDesc.Texture2D.MipSlice = 0;
	mRTVDesc.Texture2D.PlaneSlice = 0;

	mCurrentResourceState = D3D12_RESOURCE_STATE_PRESENT;
}


RenderTargetTexture::~RenderTargetTexture()
{
}
