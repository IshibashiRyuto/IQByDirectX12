#include "TextureManager.h"
#include "../Device.h"
#include <vector>
#include <algorithm>


int TextureManager::Regist(std::shared_ptr<Texture> texture)
{
	int handle = mNextTextureHandle;
	mData[handle] = texture;
	UpdateNextTextureHandle();
	return handle;
}

void TextureManager::Erase(int textureHandle)
{
	auto eraseTarget = mData.find(textureHandle);
	if (eraseTarget == mData.end())
	{
		return;
	}
	mData.erase(eraseTarget);
}

std::shared_ptr<Texture> TextureManager::GetTexture(int textureHandle) const
{
	if (!IsExist(textureHandle))
	{
		return nullptr;
	}
	return mData.at(textureHandle);
}

bool TextureManager::IsExist(int handle) const
{
	if (mData.find(handle) != mData.end())
	{
		return true;
	}
	return false;
}

TextureManager::TextureManager()
{
	mNextTextureHandle = TEXTURE_SIGNATURE;
}

void TextureManager::UpdateNextTextureHandle()
{
	do
	{
		++mNextTextureHandle;
		if( (mNextTextureHandle >> DATA_SIGNATURE_SHIFT_NUM) > (TEXTURE_SIGNATURE >> DATA_SIGNATURE_SHIFT_NUM))
		{
			mNextTextureHandle = TEXTURE_SIGNATURE;
		}
	} while (mData.find(mNextTextureHandle) != mData.end());
}

void TextureManager::CreateWhiteAndBlackTexture(std::shared_ptr<Device> device)
{
	ComPtr<ID3D12Resource> whiteTexture;
	ComPtr<ID3D12Resource> blackTexture;

	std::vector<unsigned char> whiteTextureData(4 * 4 * 4);
	std::vector<unsigned char> blackTextureData(4 * 4 * 4);
	std::fill(whiteTextureData.begin(), whiteTextureData.end(), 0xff);
	std::fill(blackTextureData.begin(), blackTextureData.end(), 0x07);
	
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC rscDesc;
	rscDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rscDesc.Alignment = 0;
	rscDesc.Width = 2;
	rscDesc.Height = 2;
	rscDesc.DepthOrArraySize = 1;
	rscDesc.MipLevels = 0;
	rscDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rscDesc.SampleDesc.Count = 1;
	rscDesc.SampleDesc.Quality = 0;
	rscDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	rscDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto result = (*device)->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&rscDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&whiteTexture));
	result = (*device)->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&rscDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&blackTexture));

	whiteTexture->WriteToSubresource(0, nullptr, whiteTextureData.data(), 4*4, 4*4*4);
	blackTexture->WriteToSubresource(0, nullptr, blackTextureData.data(), 4*4, 4*4*4);
	auto _whiteTexture = Texture::Create(whiteTexture);
	auto _blackTexture = Texture::Create(blackTexture);
	
	mData[BLACK_TEXTURE] = _blackTexture;
	mData[WHITE_TEXTURE] = _whiteTexture;
}


TextureManager::~TextureManager()
{
}
