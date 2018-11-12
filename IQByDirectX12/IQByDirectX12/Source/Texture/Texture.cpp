#include "Texture.h"
#include "WICTextureLoader\WICTextureLoader12.h"


Texture::Texture(ComPtr<ID3D12Resource> textureData)
	: mTextureData(textureData)
{
	ConstructSRV();
}

void Texture::ConstructSRV()
{
	if (!mTextureData)
	{
		return;
	}

	auto rscDesc = mTextureData->GetDesc();

	mShaderResourceView = D3D12_SHADER_RESOURCE_VIEW_DESC{};
	mShaderResourceView.Format = rscDesc.Format;
	mShaderResourceView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mShaderResourceView.Texture2D.MipLevels = rscDesc.MipLevels;
	mShaderResourceView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
}


Texture::~Texture()
{
}

std::shared_ptr<Texture> Texture::Create(ComPtr<ID3D12Resource> textureData)
{
	auto texture = std::shared_ptr<Texture>(new Texture(textureData));
	return texture;
}

const ComPtr<ID3D12Resource> Texture::GetTextureData() const
{
	return mTextureData;
}

const D3D12_SHADER_RESOURCE_VIEW_DESC & Texture::GetShaderResourceView() const
{
	return mShaderResourceView;
}

void Texture::SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC & srv)
{
	mShaderResourceView = srv;
}
