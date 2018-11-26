#include <Windows.h>
#include <d3dx12.h>
#include <iostream>
#include <DirectXTex.h>
#include "WICTextureLoader/WICTextureLoader12.h"
#include "TextureLoader.h"
#include "../Dx12/CommandQueue.h"
#include "../Dx12/GraphicsCommandList.h"
#include "../Dx12/Device.h"
#include "../ConvertString.h"
#include "../Debug/DebugLayer.h"
#include <d3d12.h>

#pragma comment(lib, "DirectXTex.lib")

using namespace DirectX;

TextureLoader::TextureLoader(std::shared_ptr<Device> device)
	: mTextureManager(TextureManager::GetInstance())
	, mDevice(device)
{
}


TextureLoader::~TextureLoader()
{
	for (auto& textureHandle : mTextureHandleManager)
	{
		mTextureManager.Erase(textureHandle.second);
	}
}

std::shared_ptr<TextureLoader> TextureLoader::Create(std::shared_ptr<Device> device)
{
	auto textureLoader = std::shared_ptr<TextureLoader>(new TextureLoader(device));
	return textureLoader;
}

int TextureLoader::Load(const std::string & str)
{
	auto wstr = ConvertStringToWString(str);
	return Load(wstr);
}

int TextureLoader::Load(const std::wstring & filePath)
{
	auto it = mTextureHandleManager.find(filePath);
	if (it == mTextureHandleManager.end() || !mTextureManager.IsExist((*it).second))
	{
		ComPtr<ID3D12Resource> resource;
		TexMetadata metaData;
		ScratchImage imageData;


		auto signature = filePath.substr(filePath.rfind(L'.')+1);
		bool isImageLoaded;

		if (signature == L"tga")
		{
			isImageLoaded = LoadTGATexture(filePath, &metaData, imageData);
		}
		else if (signature == L"dds")
		{
			isImageLoaded = LoadDDSTexture(filePath, &metaData, imageData);
		}
		else
		{
			isImageLoaded = LoadWICTexture(filePath, &metaData, imageData);
		}

		if (!isImageLoaded)
		{
			std::wstring str = L"Failed Load Texture File \"" + filePath;
			if ((*str.rbegin()) == '\0')
			{
				(*str.rbegin()) = ' ';
			}
			str += L"\".";
			DebugLayer::GetInstance().PrintDebugMessage(str);
			return -1;
		}

		auto textureData = Texture::Create(mDevice, metaData, imageData, filePath);
		if (textureData == nullptr)
		{
			return -1;
		}
		mTextureHandleManager[filePath] = mTextureManager.Regist(textureData);
	}
	return mTextureHandleManager[filePath];
}

bool TextureLoader::LoadTGATexture(std::wstring filePath, TexMetadata * metaData, ScratchImage & imageData) const
{
	auto result = LoadFromTGAFile(filePath.c_str(), metaData, imageData);
	return static_cast<bool>(SUCCEEDED(result));
}

bool TextureLoader::LoadDDSTexture(std::wstring filePath, DirectX::TexMetadata * metaData, DirectX::ScratchImage & imageData) const
{
	auto result = LoadFromDDSFile(filePath.c_str(), WIC_FLAGS_NONE, metaData, imageData);
	return static_cast<bool>(SUCCEEDED(result));
}

bool TextureLoader::LoadWICTexture(std::wstring filePath, TexMetadata * metaData, ScratchImage & imageData) const
{
	auto result = LoadFromWICFile(filePath.c_str(), WIC_FLAGS_NONE, metaData, imageData);
	return static_cast<bool>(SUCCEEDED(result));
}


