#include <Windows.h>
#include <d3dx12.h>
#include <iostream>
#include <DirectXTex.h>
#include "WICTextureLoader/WICTextureLoader12.h"
#include "TextureLoader.h"
#include "../CommandQueue.h"
#include "../GraphicsCommandList.h"
#include "../Device.h"
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

		resource = CreateTextureResource(metaData, imageData);
		
		if (!resource)
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

		auto textureData = Texture::Create(resource);
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

ComPtr<ID3D12Resource> TextureLoader::CreateTextureResource(const TexMetadata & metaData, const ScratchImage & imageData)
{
	ComPtr<ID3D12Resource> texResource;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = metaData.width;
	resourceDesc.Height = metaData.height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = metaData.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto result = (*mDevice)->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texResource));

	if (FAILED(result))
	{
		return nullptr;
	}
	
	D3D12_BOX writeBox = {};
	writeBox.left = 0;
	writeBox.right = (metaData.width);
	writeBox.top = 0;
	writeBox.bottom = (metaData.height);
	writeBox.front = 0;
	writeBox.back = 1;

	texResource->WriteToSubresource(0, &writeBox, imageData.GetPixels(), 4 * metaData.width, 4 * metaData.width * metaData.height);
	
	return texResource;
}

