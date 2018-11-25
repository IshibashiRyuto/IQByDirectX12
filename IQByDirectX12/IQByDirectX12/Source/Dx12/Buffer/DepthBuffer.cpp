#include "DepthBuffer.h"
#include <d3dx12.h>
#include "../DescriptorHeap.h"
#include "../GraphicsCommandList.h"


DepthBuffer::DepthBuffer(std::shared_ptr<Device> device, unsigned int width, unsigned int height, const D3D12_CLEAR_VALUE & clearValue, const std::wstring & bufferName, HRESULT & result)
	: Dx12Resource
	(
		device,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R32_TYPELESS,
			static_cast<UINT>(width),
			static_cast<UINT>(height),
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
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

	mDepthDescHeap = DescriptorHeap::Create(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	if ( mDepthDescHeap == nullptr)
	{
		result = E_FAIL;
		return;
	}

	ConstructDSV(device);
}

DepthBuffer::~DepthBuffer()
{
}

std::shared_ptr<DepthBuffer> DepthBuffer::Create(std::shared_ptr<Device> device, unsigned int width, unsigned int height, const std::wstring & bufferName, bool isAlignment, float depthClearValue)
{
	if (isAlignment)
	{
		width = height = Alignment(max(width, height));
	}
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = depthClearValue;
	HRESULT result;
	auto ret = std::shared_ptr<DepthBuffer>(new DepthBuffer(device, width, height, clearValue, bufferName, result));
	if (FAILED(result))
	{
		return nullptr;
	}
	return ret;
}

void DepthBuffer::ClearDepthBuffer(std::shared_ptr<GraphicsCommandList> commandList)
{
	(*commandList)->ClearDepthStencilView(mDepthDescHeap->GetCPUHandle(0), D3D12_CLEAR_FLAG_DEPTH, CLEAR_VALUE.DepthStencil.Depth, 0, 0, nullptr);
}

void DepthBuffer::BeginWriteDepth(std::shared_ptr<GraphicsCommandList> commandList)
{

	TransitionState(commandList, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	ClearDepthBuffer(commandList);
}

void DepthBuffer::EndWriteDepth(std::shared_ptr<GraphicsCommandList> commandList)
{
	TransitionState(commandList,D3D12_RESOURCE_STATE_DEPTH_READ);
}

const D3D12_DEPTH_STENCIL_VIEW_DESC & DepthBuffer::GetDSVDesc() const
{
	return mDSVDesc;
}

unsigned int DepthBuffer::Alignment(unsigned int num)
{
	unsigned int alignmentDigit = 0;
	num = min(num, num - 1);
	for (unsigned int i = static_cast<unsigned int>(sizeof(unsigned int) * 4);
		i > 0; --i)
	{
		if (((0x01 << i) & num) != 0)
		{
			return 0x01 << i;
		}
	}

}

void DepthBuffer::ConstructDSV(std::shared_ptr<Device> device)
{
	mDSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
	mDSVDesc.Format = DXGI_FORMAT_D32_FLOAT;

	mDepthDescHeap->SetDepthStencilView(mDSVDesc, mResource, 0);
}
