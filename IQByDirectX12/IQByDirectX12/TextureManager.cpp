#include "TextureManager.h"



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
	try
	{
		auto texture = mData.at(textureHandle);
		return texture;
	}
	catch (std::out_of_range &)
	{
		return nullptr;
	}
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
	: mNextTextureHandle(TEXTURE_SIGNATURE)
{
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


TextureManager::~TextureManager()
{
}
