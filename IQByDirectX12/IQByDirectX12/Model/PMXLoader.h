#pragma once
#include <cstdio>
#include <string>
#include "../Math/Math.h"

// システムヘッダインクルード
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// 自作ヘッダインクルード
#include "PMXModelData.h"

// クラス使用宣言
using Microsoft::WRL::ComPtr;

class PMXLoader
{
public:
	PMXLoader();
	~PMXLoader();


	static std::shared_ptr<PMXLoader> Create(ComPtr<ID3D12Device> device);

	std::shared_ptr<PMXModelData> LoadModel(const std::string& filePath);

private:
	ComPtr<ID3D12Device> mDevice;
	const std::string FILE_SIGNATURE = "PMX ";
	const size_t SIGNATURE_SIZE = sizeof(char) * 4;

	std::string ReadTextBuf(FILE *fp);
};

