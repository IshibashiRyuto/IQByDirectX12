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
#include <map>

// 自作ヘッダインクルード
#include "ModelDataManager.h"
#include "Model.h"

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class ModelData;
class Device;
class TextureLoader;
class PipelineStateObject;
class RootSignature;

class ModelLoader
{
public:
	
	/// デストラクタ
	virtual ~ModelLoader();

	/// @fn LoadModel
	/// モデルをロードする
	/// @param[in] filePath	: ファイルパス
	virtual std::shared_ptr<Model> LoadModel(const std::string& filePath, std::shared_ptr<PipelineStateObject> pso,
		std::shared_ptr<PipelineStateObject> shadowPSO, std::shared_ptr<RootSignature> rootSignature) = 0;

	/// @fn ClearModelData
	/// 読み込んだモデル情報を削除する
	virtual void ClearModelData() = 0;

protected:
	/* 変数宣言 */
	std::shared_ptr<Device> mDevice;			// Dx12デバイス
	ModelDataManager& mModelDataManager;	// モデルデータマネージャへの参照
	std::map<std::string, int> mModelHandleManager;	// モデルハンドルの管理
	std::shared_ptr<TextureLoader> mTextureLoader;		// モデルテクスチャローダ
													
	/// コンストラクタ
	ModelLoader(std::shared_ptr<Device> device);
};

