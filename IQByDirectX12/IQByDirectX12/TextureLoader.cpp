#include <Windows.h>
#include <d3dx12.h>
#include <iostream>
#include "TextureLoader.h"
#include "WICTextureLoader/WICTextureLoader12.h"
#include"CommandAllocator.h"
#include "CommandQueue.h"

TextureLoader::TextureLoader()
	: mTextureManager(TextureManager::GetInstance())
{
}


TextureLoader::~TextureLoader()
{
}

std::shared_ptr<TextureLoader> TextureLoader::Create(ComPtr<ID3D12Device> device)
{
	auto textureLoader = std::make_shared<TextureLoader>();
	textureLoader->mDevice = device;
	return textureLoader;
}

int TextureLoader::Load(const std::string & str)
{
	auto it = mTextureHandleManager.find(str);
	if(it == mTextureHandleManager.end() || !mTextureManager.IsExist((*it).second))
	{
		auto wstr = GetWString(str);
		ComPtr<ID3D12Resource> resource;
		
		D3D12_SUBRESOURCE_DATA subResourceData;
		std::unique_ptr<uint8_t[]> decodedData;
		auto result = DirectX::LoadWICTextureFromFile(mDevice.Get(),
			wstr.data(),
			&resource,
			decodedData,
			subResourceData);

		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Load Texture File \"" << str << "\".";
#endif
			return -1;
		}

		UpdateTextureSubresource(resource, subResourceData);

		auto textureData = Texture::Create(resource);
		mTextureHandleManager[str] = mTextureManager.Regist(textureData);
	}
	return mTextureHandleManager[str];
}

std::wstring TextureLoader::GetWString(const std::string & str)
{
	// ワイド文字列のバイトサイズを取得
	auto byteSize = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.data(), -1, nullptr, 0);

	// 変換先の文字列バッファを生成
	std::wstring wstr;
	wstr.resize(byteSize);

	byteSize = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.data(), -1, &wstr[0], byteSize);

	return wstr;
}

void TextureLoader::UpdateTextureSubresource(ComPtr<ID3D12Resource> resource,D3D12_SUBRESOURCE_DATA & subresource)
{
	ComPtr<ID3D12Resource> updateBuffer;

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

	mDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&uploadDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&updateBuffer)
	);

	auto commandQueue = CommandQueue::Create(mDevice);
	auto commandAllocator = CommandAllocator::Create(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);

	ComPtr<ID3D12GraphicsCommandList> commandList;
	auto result = mDevice->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator->Get().Get(),
		nullptr,
		IID_PPV_ARGS(&commandList));


	UpdateSubresources(commandList.Get(),
		resource.Get(),
		updateBuffer.Get(),
		(UINT64)0,
		(UINT)0,
		(UINT)1,
		&subresource);

	commandList->Close();

	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandList(_countof(commandLists), commandLists);
	commandQueue->Signal();
}
