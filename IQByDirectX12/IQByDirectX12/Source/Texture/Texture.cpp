#include "Texture.h"
#include "WICTextureLoader\WICTextureLoader12.h"


Texture::Texture(ComPtr<ID3D12Resource> textureData)
	: mTextureData(textureData)
{
	ConstructSRVDesc();
}

void Texture::ConstructSRVDesc()
{
	if (!mTextureData)
	{
		return;
	}

	auto rscDesc = mTextureData->GetDesc();

	mShaderResourceViewDesc = D3D12_SHADER_RESOURCE_VIEW_DESC{};
	mShaderResourceViewDesc.Format = rscDesc.Format;
	mShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mShaderResourceViewDesc.Texture2D.MipLevels = rscDesc.MipLevels;
	mShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
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

const D3D12_SHADER_RESOURCE_VIEW_DESC & Texture::GetShaderResourceViewDesc() const
{
	return mShaderResourceViewDesc;
}

void Texture::SetShaderResourceViewDesc(const D3D12_SHADER_RESOURCE_VIEW_DESC & srvDesc)
{
	mShaderResourceViewDesc = srvDesc;
}
