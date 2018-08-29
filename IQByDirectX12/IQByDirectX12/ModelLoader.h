/*
	ModelLoader
	モデルローダーのベース
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include <functional>

// 自作ヘッダインクルード

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class ModelData;

class ModelLoader
{
public:
	/// コンストラクタ
	ModelLoader(ComPtr<ID3D12Device> mDevice);
	
	/// デストラクタ
	~ModelLoader();

	/// @fn LoadModel
	/// モデルをロードする
	virtual std::shared_ptr<ModelData> LoadModel(const std::string& filePath) = 0;

protected:
	ComPtr<ID3D12Device> mDevice;
};

