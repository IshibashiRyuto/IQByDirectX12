#include <iostream>
#include "PMDLoader.h"


PMDLoader::PMDLoader(ComPtr<ID3D12Device> device)
	: ModelLoader(device)
{
}


PMDLoader::~PMDLoader()
{
	ClearModelData();
}

std::shared_ptr<PMDLoader> PMDLoader::Create(ComPtr<ID3D12Device> device)
{
	auto pmdLoader = std::shared_ptr<PMDLoader>(new PMDLoader(device));
	pmdLoader->mDevice = device;
	return pmdLoader;
}

std::shared_ptr<Model> PMDLoader::LoadModel(const std::string & filePath)
{
	auto handleItr = mModelHandleManager.find(filePath);
	

	// ファイルパスに対応するモデルハンドルが存在しない
	// or モデルハンドルに対応するモデルデータが消滅している
	if (handleItr == mModelHandleManager.end() || !mModelDataManager.IsExist((*handleItr).second))
	{
		// モデル読み込み

		unsigned int vertexCount;
		std::vector<PMDVertex> vertex;
		unsigned int indexCount;
		std::vector<unsigned short> index;
		unsigned int materialCount;
		std::vector<PMDMaterial> materials;
		PMDHeader pmdHeader;

		// ファイルを開く
		FILE *fp;
		fopen_s(&fp, filePath.c_str(), "rb");

		if (fp == nullptr)
		{
			return nullptr;
		}

		// ファイルフォーマットの確認
		{
			char pmdSignature[4];
			fread(pmdSignature, sizeof(char) * 3, 1, fp);
			pmdSignature[3] = '\0';
			if (pmdSignature == nullptr || strcmp(pmdSignature, "Pmd") != 0)
			{
				std::cout << "This file is not pmd.\n Please load \".pmd\" file." << std::endl;
				fclose(fp);
				return nullptr;
			}
		}

		// ヘッダデータ読み込み
		fread(&pmdHeader, sizeof(pmdHeader), 1, fp);

		// 頂点情報の読み込み
		fread(&vertexCount, sizeof(int), 1, fp);
		vertex.resize(vertexCount);
		fread(vertex.data(), sizeof(PMDVertex), vertex.size(), fp);

		// 頂点インデックス情報読み込み
		fread(&indexCount, sizeof(int), 1, fp);
		index.resize(indexCount);
		fread(index.data(), sizeof(short), index.size(), fp);

		// マテリアルパラメータ読み込み
		fread(&materialCount, sizeof(int), 1, fp);
		materials.resize(materialCount);
		fread(materials.data(), sizeof(PMDMaterial), materials.size(), fp);

		fclose(fp);

		/// モデルデータの生成、登録
		auto modelData = PMDModelData::Create(mDevice, vertex, index, materials);

		mModelHandleManager[filePath] = mModelDataManager.Regist(modelData);
	}

	return std::shared_ptr<Model>(new Model(mModelHandleManager[filePath]));
}

void PMDLoader::ClearModelData()
{
	for (auto& modelHandlePair : mModelHandleManager)
	{
		mModelDataManager.Erase(modelHandlePair.second);
	}
}
