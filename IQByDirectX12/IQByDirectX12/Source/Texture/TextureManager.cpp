#include "TextureManager.h"
#include "../Dx12/Device.h"
#include <d3dx12.h>
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
	std::vector<unsigned char> whiteTextureData(4 * 4 * 4);
	std::vector<unsigned char> blackTextureData(4 * 4 * 4);
	std::fill(whiteTextureData.begin(), whiteTextureData.end(), 0xff);
	std::fill(blackTextureData.begin(), blackTextureData.end(), 0x07);

	auto whiteTexture = Texture::Create(device, 4, 4, DXGI_FORMAT_R8G8B8A8_UNORM, L"WhiteTexture");
	auto blackTexture = Texture::Create(device, 4, 4, DXGI_FORMAT_R8G8B8A8_UNORM, L"BlackTexture");

	whiteTexture->WriteTextureData(whiteTextureData.data(), 4, 4, 4);
	blackTexture->WriteTextureData(blackTextureData.data(), 4, 4, 4);

	
	mData[BLACK_TEXTURE] = blackTexture;
	mData[WHITE_TEXTURE] = whiteTexture;
}


TextureManager::~TextureManager()
{
}
