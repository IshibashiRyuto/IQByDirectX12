#pragma once
// システムヘッダインクルード
#include <memory>

// 自作ヘッダインクルード
#include "PMDModelData.h"


class PMDLoader
{
public:
	~PMDLoader();

	static std::shared_ptr<PMDLoader> Create();

	std::shared_ptr<PMDModelData> LoadModel(const std::string& filePath);
private:
	PMDLoader();
	PMDLoader(const PMDLoader&);
	void operator=(const PMDLoader&) {}
};

