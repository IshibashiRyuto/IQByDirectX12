#pragma once
// システムヘッダインクルード
#include <memory>

// 自作ヘッダインクルード
#include "ModelLoader.h"
#include "PMDModelData.h"


class PMDLoader : public ModelLoader
{
public:
	~PMDLoader();

	static std::shared_ptr<PMDLoader> Create(std::shared_ptr<Device> device);

	/// @fn LoadModel
	std::shared_ptr<Model> LoadModel(const std::string& filePath);

	/// @fn ClearModelData
	/// 読み込んだモデル情報を削除する
	void ClearModelData();

private:
	PMDLoader(std::shared_ptr<Device> device);
	PMDLoader(const PMDLoader&);
	void operator=(const PMDLoader&) {}
};

