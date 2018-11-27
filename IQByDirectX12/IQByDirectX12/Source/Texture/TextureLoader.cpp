#include <Windows.h>
#include <d3dx12.h>
#include <iostream>
#include "TextureLoader.h"
#include "WICTextureLoader/WICTextureLoader12.h"
#include "../CommandQueue.h"
#include "../GraphicsCommandList.h"
#include "../Device.h"
#include "../ConvertString.h"
#include "../Debug/DebugLayer.h"

TextureLoader::TextureLoader(std::shared_ptr<Device> device)
	: mTextureManager(TextureManager::GetInstance())
	, mDevice(device)
{
	mCommandList = GraphicsCommandList::Create(device, D3D12_COMMAND_LIST_TYPE_DIRECT, L"TextureLoader");
	mCommandQueue = CommandQueue::Create(device);
	mCommandList->Close();
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
		
		D3D12_SUBRESOURCE_DATA subResourceData;
		std::unique_ptr<uint8_t[]> decodedData;
		auto result = DirectX::LoadWICTextureFromFile(mDevice->GetDevice().Get(),
			filePath.data(),
			&resource,
			decodedData,
			subResourceData);

		if (FAILED(result))
		{
			DebugLayer::GetInstance().PrintDebugMessage(L"Failed Load Texture File \"" + filePath + L"\".");
			return -1;
		}

		UpdateTextureSubresource(resource, subResourceData);

		auto textureData = Texture::Create(resource);
		mTextureHandleManager[filePath] = mTextureManager.Regist(textureData);
	}
	return mTextureHandleManager[filePath];
}

void TextureLoader::UpdateTextureSubresource(ComPtr<ID3D12Resource> resource,D3D12_SUBRESOURCE_DATA & subresource)
{
	auto format = resource->GetDesc().Format;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC uploadDesc;
	uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	uploadDesc.Alignment = 0;
	uploadDesc.Width = GetRequiredIntermediateSize(resource.Get(), 0, 1);
	uploadDesc.Height = 1;
	uploadDesc.DepthOrArraySize = 1;
	uploadDesc.MipLevels = 1;
	uploadDesc.Format = DXGI_FORMAT_UNKNOWN;
	uploadDesc.SampleDesc.Count = 1;
	uploadDesc.SampleDesc.Quality = 0;
	uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	uploadDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	(*mDevice)->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&uploadDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mUpdateBuffer)
	);


	// リソースアップデートコマンドの実行
	
	mCommandList->Reset();

	UpdateSubresources( mCommandList->GetCommandList().Get(),
		resource.Get(),
		mUpdateBuffer.Get(),
		(UINT64)0,
		(UINT)0,
		(UINT)1,
		&subresource);

	mCommandList->Close();

	mCommandQueue->ExecuteCommandList(mCommandList);
	mCommandQueue->Signal();
}
