#include <d3dx12.h>
#include "Texture.h"
#include "DepthBuffer.h"
#include "RenderTargetBuffer.h"
#include "../GraphicsCommandList.h"



Texture::Texture(std::shared_ptr<Device> device, const DirectX::TexMetadata & metaData, const std::wstring & textureName, HRESULT& result)
	: Dx12Resource
	(
		device,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Tex2D
		(
			metaData.format,
			static_cast<UINT64>(metaData.width),
			static_cast<UINT64>(metaData.height),
			static_cast<UINT16>(metaData.arraySize),
			static_cast<UINT16>(metaData.mipLevels)),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		textureName,
		result
	)
	, mIsUpdate(false)
	, TEXTURE_SIZE(metaData.width,metaData.height)
{
	if (FAILED(result))
	{
		return;
	}
	mTemporaryResource = Dx12Resource::Create(device,
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Tex2D
		(
			metaData.format,
			static_cast<UINT64>(metaData.width),
			static_cast<UINT64>(metaData.height),
			static_cast<UINT16>(metaData.arraySize),
			static_cast<UINT16>(metaData.mipLevels)),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		textureName + L"Copy");
	if (!mTemporaryResource)
	{
		result = E_FAIL;
		return;
	}
	ConstructSRVDesc();
}

Texture::Texture(std::shared_ptr<Device> device, unsigned int width, unsigned int height, DXGI_FORMAT format, const std::wstring& textureName, HRESULT& result)
	: Dx12Resource
	(
		device,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Tex2D
		(
			format,
			static_cast<UINT64>(width),
			static_cast<UINT64>(height)
			),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		textureName,
		result
	)
	, mIsUpdate(false)
	, TEXTURE_SIZE(width, height)
{
	if (FAILED(result))
	{
		return;
	}
	mTemporaryResource = Dx12Resource::Create(device,
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Tex2D
		(
			format,
			static_cast<UINT64>(width),
			static_cast<UINT64>(height)),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		textureName + L"Copy");
	ConstructSRVDesc();
}

Texture::Texture(std::shared_ptr<Dx12Resource> resource)
	: Dx12Resource(resource)
	, TEXTURE_SIZE({})
	, mIsUpdate(false)
{
	ConstructSRVDesc();
}

Texture::~Texture()
{
}

std::shared_ptr<Texture> Texture::Create(std::shared_ptr<Device> device, const DirectX::TexMetadata & metaData, const DirectX::ScratchImage & texImage, const std::wstring& textureName)
{
	HRESULT result;
	auto ret = std::shared_ptr<Texture>(new Texture(device, metaData, textureName, result));
	if (FAILED(result))
	{
		return nullptr;
	}
	ret->WriteTextureData(metaData, texImage);
	return ret;
}

std::shared_ptr<Texture> Texture::Create(std::shared_ptr<Device> device, unsigned int width, unsigned int height, DXGI_FORMAT format, const std::wstring & textureName)
{
	HRESULT result;
	auto ret = std::shared_ptr<Texture>(new Texture(device, width, height, format, textureName, result));
	if (FAILED(result))
	{
		return nullptr;
	}
	return ret;
}

std::shared_ptr<Texture> Texture::Create(std::shared_ptr<DepthBuffer> depthBuffer)
{
	return std::shared_ptr<Texture>(new Texture(depthBuffer));
}


std::shared_ptr<Texture> Texture::Create(std::shared_ptr<RenderTargetBuffer> renderTargetBuffer)
{
	return std::shared_ptr<Texture>(new Texture(renderTargetBuffer));
}

void Texture::WriteTextureData(const DirectX::TexMetadata & metaData, const DirectX::ScratchImage & texImage)
{
	WriteTextureData(texImage.GetPixels(), metaData.width, metaData.height, static_cast<unsigned int>(texImage.GetPixelsSize() / metaData.width / metaData.height));
	//D3D12_BOX writeBox = {};
	//writeBox.left = 0;
	//writeBox.right = static_cast<UINT>(metaData.width);
	//writeBox.top = 0;
	//writeBox.bottom = static_cast<UINT>(metaData.height);
	//writeBox.front = 0;
	//writeBox.back = 1;
	//// TODO: ピクセル当たりのデータサイズ調査
	//// 現状、4バイト固定になってるため、RGBテクスチャとか読み込めないので注意
	//mResource->WriteToSubresource(0, &writeBox, texImage.GetPixels(), static_cast<UINT>(4 * metaData.width), static_cast<UINT>(4 * metaData.width * metaData.height));
}

void Texture::WriteTextureData(void * pImage, unsigned int width, unsigned int height, size_t pixelDataSize)
{
	D3D12_BOX writeBox = {};
	writeBox.left = 0;
	writeBox.right = static_cast<UINT>(width);
	writeBox.top = 0;
	writeBox.bottom = static_cast<UINT>(height);
	writeBox.front = 0;
	writeBox.back = 1;

	mRowPitch = pixelDataSize * width;

	mTemporaryResource->GetResource()->WriteToSubresource(0, &writeBox, pImage, static_cast<UINT>(mRowPitch), static_cast<UINT>(mRowPitch * height));
	mIsUpdate = true;
}

const D3D12_SHADER_RESOURCE_VIEW_DESC & Texture::GetShaderResourceViewDesc() const
{
	return mShaderResourceViewDesc;
}

void Texture::UpdateTexture(std::shared_ptr<GraphicsCommandList> commandList)
{
	if (mIsUpdate)
	{
		mTemporaryResource->TransitionState(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE);
		TransitionState(commandList, D3D12_RESOURCE_STATE_COPY_DEST);

		/*D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrint;
		footPrint.Offset = 0;
		footPrint.Footprint = CD3DX12_SUBRESOURCE_FOOTPRINT(mResource->GetDesc(), mRowPitch);
		auto dstLocation = CD3DX12_TEXTURE_COPY_LOCATION(mResource.Get(),footPrint);

		(*commandList)->CopyTextureRegion(
			&CD3DX12_TEXTURE_COPY_LOCATION(mResource.Get(), footPrint),
			0, 0, 0,
			&CD3DX12_TEXTURE_COPY_LOCATION(mTemporaryResource->GetResource().Get()),
			&CD3DX12_BOX(0, 0, TEXTURE_SIZE.x, TEXTURE_SIZE.y)
		);*/

		(*commandList)->CopyResource(mResource.Get(), mTemporaryResource->GetResource().Get());

		mTemporaryResource->TransitionState(commandList, D3D12_RESOURCE_STATE_COMMON);
		TransitionState(commandList, D3D12_RESOURCE_STATE_COMMON);
		mIsUpdate = false;
	}
}

void Texture::ConstructSRVDesc()
{
	if (!mResource)
	{
		return;
	}
	auto rscDesc = mResource->GetDesc();

	auto format = rscDesc.Format;

	//	R32_TYPELESSをR32_FLOATに変換(深度バッファ用)
	if (format == DXGI_FORMAT_R32_TYPELESS)
	{
		format = DXGI_FORMAT_R32_FLOAT;
	}

	mShaderResourceViewDesc = D3D12_SHADER_RESOURCE_VIEW_DESC{};
	mShaderResourceViewDesc.Format = format;
	mShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mShaderResourceViewDesc.Texture2D.MipLevels = rscDesc.MipLevels;
	mShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
}
