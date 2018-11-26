#include "RenderTargetBuffer.h"
#include <d3dx12.h>

RenderTargetBuffer::RenderTargetBuffer(
	std::shared_ptr<Device> device,
	unsigned int width,
	unsigned int height,
	const std::wstring& bufferName,
	DXGI_FORMAT format,
	const D3D12_CLEAR_VALUE & clearValue,
	HRESULT & result)
	: Dx12Resource(
		device,
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Tex2D(format, static_cast<UINT64>(width),
			static_cast<UINT64>(height),
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
		D3D12_RESOURCE_STATE_PRESENT,
		&clearValue,
		bufferName,
		result
	)
	, CLEAR_VALUE(clearValue)
{
	if (FAILED(result))
	{
		return;
	}
	ConstructRTVDesc();
}

RenderTargetBuffer::RenderTargetBuffer(ComPtr<ID3D12Resource> buffer, D3D12_RESOURCE_STATES state, const D3D12_CLEAR_VALUE& clearValue)
	: Dx12Resource(buffer,state)
	, CLEAR_VALUE(clearValue)
{
	ConstructRTVDesc();
}

RenderTargetBuffer::~RenderTargetBuffer()
{
}

std::shared_ptr<RenderTargetBuffer> RenderTargetBuffer::Create(std::shared_ptr<Device> device, unsigned int width, unsigned int height, const std::wstring& bufferName, DXGI_FORMAT format, Math::Vector4 clearColor)
{
	HRESULT result;
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;
	auto ret = std::shared_ptr<RenderTargetBuffer>(new RenderTargetBuffer(device, width, height, bufferName, format, clearValue, result));
	if (FAILED(result))
	{
		return nullptr;
	}
	return ret;
}

std::shared_ptr<RenderTargetBuffer> RenderTargetBuffer::Create(ComPtr<ID3D12Resource> buffer, D3D12_RESOURCE_STATES state)
{
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = buffer->GetDesc().Format;
	clearValue.Color[0] = 0.f;
	clearValue.Color[1] = 0.f;
	clearValue.Color[2] = 1.f;
	clearValue.Color[3] = 1.f;
	return std::shared_ptr<RenderTargetBuffer>(new RenderTargetBuffer(buffer, state, clearValue));
}

const D3D12_RENDER_TARGET_VIEW_DESC & RenderTargetBuffer::GetRenderTargetViewDesc() const
{
	return mRTVDesc;
}

const D3D12_CLEAR_VALUE & RenderTargetBuffer::GetClearValue() const
{
	return CLEAR_VALUE;
}

void RenderTargetBuffer::ConstructRTVDesc()
{
	auto rscDesc = mResource->GetDesc();
	mRTVDesc.Format = rscDesc.Format;
	mRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	mRTVDesc.Texture2D.MipSlice = 0;
	mRTVDesc.Texture2D.PlaneSlice = 0;
}
