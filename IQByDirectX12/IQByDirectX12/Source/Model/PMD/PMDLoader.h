#pragma once
// システムヘッダインクルード
#include <memory>

// 自作ヘッダインクルード
#include "../ModelLoader.h"
#include "PMDModelData.h"


class PMDLoader : public ModelLoader
{
public:
	~PMDLoader();

	static std::shared_ptr<PMDLoader> Create(std::shared_ptr<Device> device, const std::string& shareToonFolderPath);

	/// @fn LoadModel
	std::shared_ptr<Model> LoadModel(const std::string& filePath, std::shared_ptr<PipelineStateObject> pso,
		std::shared_ptr<PipelineStateObject> shadowPSO, std::shared_ptr<RootSignature> rootSignature);

	/// @fn ClearModelData
	/// 読み込んだモデル情報を削除する
	void ClearModelData();

private:
	PMDLoader(std::shared_ptr<Device> device);
	PMDLoader(const PMDLoader&);

	const int SHARE_TOON_NUM = 11;
	const std::string SHARE_TOON_PATH[11]
	{
		"toon0.bmp",
		"toon01.bmp",
		"toon02.bmp",
		"toon03.bmp",
		"toon04.bmp",
		"toon05.bmp",
		"toon06.bmp",
		"toon07.bmp",
		"toon08.bmp",
		"toon09.bmp",
		"toon10.bmp",
	};		

	std::vector<int> mShareToonTextureHandle;
	
	void operator=(const PMDLoader&) {}

	void LoadShareToon(const std::string& toonFolderPath);

	void LoadToonData(FILE *fp, std::vector<int>& toonTextureHandle, const std::string& modelPath);
};

