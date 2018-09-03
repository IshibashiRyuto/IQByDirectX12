#include "Texture.h"
#include "../WICTextureLoader\WICTextureLoader12.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

std::shared_ptr<Texture> Texture::Create(ComPtr<ID3D12Resource> textureData)
{
	auto texture = std::make_shared<Texture>();
	texture->mTextureData = textureData;
	auto rscDesc = textureData->GetDesc();
	texture->mShaderResourceView = D3D12_SHADER_RESOURCE_VIEW_DESC{};
	texture->mShaderResourceView.Format = rscDesc.Format;
	texture->mShaderResourceView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	texture->mShaderResourceView.Texture2D.MipLevels = rscDesc.MipLevels;
	texture->mShaderResourceView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

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
