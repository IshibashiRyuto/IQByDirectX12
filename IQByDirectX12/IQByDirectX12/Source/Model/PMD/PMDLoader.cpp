#include <iostream>
#include "PMDLoader.h"
#include "../Texture/TextureLoader.h"


PMDLoader::PMDLoader(std::shared_ptr<Device> device)
	: ModelLoader(device)
{
}

void PMDLoader::LoadBone(FILE *fp)
{
}

void PMDLoader::LoadShareToon(const std::string & toonFolderPath)
{
	mShareToonTextureHandle.resize(SHARE_TOON_NUM);
	for (int i = 0; i < SHARE_TOON_NUM; ++i)
	{
		auto path = toonFolderPath + "/" + SHARE_TOON_PATH[i];
		mShareToonTextureHandle[i] = mTextureLoader->Load(path);
	}
}


PMDLoader::~PMDLoader()
{
	ClearModelData();
}

std::shared_ptr<PMDLoader> PMDLoader::Create(std::shared_ptr<Device> device, const std::string& shareToonFolderPath)
{
	auto pmdLoader = std::shared_ptr<PMDLoader>(new PMDLoader(device));
	pmdLoader->LoadShareToon(shareToonFolderPath);
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

		PMDModelInfo modelInfo;

		unsigned int vertexCount;
		unsigned int indexCount;
		unsigned int materialCount;
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

		modelInfo.modelPath = filePath;

		// ヘッダデータ読み込み
		fread(&pmdHeader, sizeof(pmdHeader), 1, fp);

		// 頂点情報の読み込み
		fread(&vertexCount, sizeof(int), 1, fp);
		modelInfo.vertexData.resize(vertexCount);
		fread(modelInfo.vertexData.data(), sizeof(PMDVertex), vertexCount, fp);

		// 頂点インデックス情報読み込み
		fread(&indexCount, sizeof(int), 1, fp);
		modelInfo.indexData.resize(indexCount);
		fread(modelInfo.indexData.data(), sizeof(short), indexCount, fp);

		// マテリアルパラメータ読み込み
		fread(&materialCount, sizeof(int), 1, fp);
		modelInfo.materials.resize(materialCount);
		fread(modelInfo.materials.data(), sizeof(PMDMaterial), materialCount, fp);

		// ボーン情報読み込み


		fclose(fp);

		/// モデルデータの生成、登録
		auto modelData = PMDModelData::Create(mDevice, modelInfo, mShareToonTextureHandle);

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
