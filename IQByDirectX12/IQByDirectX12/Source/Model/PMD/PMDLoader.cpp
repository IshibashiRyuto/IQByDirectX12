#include <iostream>
#include "PMDLoader.h"
#include "../Texture/TextureLoader.h"
#include <array>


PMDLoader::PMDLoader(std::shared_ptr<Device> device)
	: ModelLoader(device)
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

void PMDLoader::LoadToonData(FILE * fp, std::vector<int>& toonTextureHandle, const std::string& modelPath)
{
	fseek(fp, 0, SEEK_SET);
	// 先頭からヘッダ情報を読み飛ばし
	fseek(fp, 283, SEEK_CUR);

	// 頂点情報を読み飛ばし
	unsigned int vertCount = 0;
	fread(&vertCount, sizeof(vertCount), 1, fp);
	fseek(fp, vertCount * 38, SEEK_CUR);

	// 頂点インデックスを読み飛ばす
	unsigned int indexCount = 0;
	fread(&indexCount, sizeof(indexCount), 1, fp);
	fseek(fp, indexCount * sizeof(short), SEEK_CUR);
	
	// マテリアル情報を読み飛ばす
	unsigned int materialCount = 0;
	fread(&materialCount, sizeof(materialCount), 1, fp);
	fseek(fp, materialCount * sizeof(PMDMaterial), SEEK_CUR);

	unsigned short boneCount;
	fread(&boneCount, sizeof(boneCount), 1, fp);
	fseek(fp, boneCount * 39, SEEK_CUR);

	// IK情報を読み飛ばす
	unsigned short ikDataCount;
	fread(&ikDataCount, sizeof(ikDataCount), 1, fp);
	for (unsigned short i = 0; i < ikDataCount; ++i)
	{
		unsigned char ikChainLength;
		fseek(fp, 4, SEEK_CUR);
		fread(&ikChainLength, sizeof(ikChainLength), 1, fp);
		fseek(fp, 6 + ikChainLength * sizeof(unsigned short), SEEK_CUR);
	}

	// 表情データを読み飛ばす
	unsigned short skinCount = 0;
	fread(&skinCount, sizeof(skinCount), 1, fp);
	for (int i = 0; i < skinCount; ++i)
	{
		fseek(fp, 20, SEEK_CUR);
		unsigned int vertNum = 0;
		fread(&vertNum, sizeof(vertNum), 1, fp);
		fseek(fp, 1, SEEK_CUR);
		fseek(fp, 16 * vertNum, SEEK_CUR);
	}

	// 表示用データ
	unsigned char skinDispNum = 0;
	fread(&skinDispNum, sizeof(skinDispNum), 1, fp);
	fseek(fp, skinDispNum * sizeof(unsigned short), SEEK_CUR);

	// 表示用ボーン名
	unsigned char boneDispNum = 0;
	fread(&boneDispNum, sizeof(boneDispNum), 1, fp);
	fseek(fp, 50 * boneDispNum, SEEK_CUR);

	// 表示ボーンリスト
	unsigned int dispBoneNum = 0;
	fread(&dispBoneNum, sizeof(dispBoneNum), 1, fp);
	fseek(fp, 3 * dispBoneNum, SEEK_CUR);

	// 英名
	unsigned char englishFlg = 0;
	fread(&englishFlg, sizeof(englishFlg), 1, fp);
	if (englishFlg)
	{
		fseek(fp, 20 + 256, SEEK_CUR);

		fseek(fp, boneCount * 20, SEEK_CUR);

		fseek(fp, (skinCount - 1) * 20, SEEK_CUR);

		fseek(fp, boneDispNum * 50, SEEK_CUR);
	}

	std::array<char[100], 10> toonTexNames;
	fread(toonTexNames.data(), sizeof(char) * 100, toonTexNames.size(), fp);
	toonTextureHandle.resize(toonTexNames.size() + 1);
	toonTextureHandle[0] = TextureManager::WHITE_TEXTURE;

	for (int i = 0; i < toonTexNames.size(); ++i)
	{
		std::string toonPath(toonTexNames[i]);
		toonPath = modelPath.substr(0, modelPath.rfind('/') + 1) + toonPath;
		int toonHandle = mTextureLoader->Load(toonPath);
		if (toonHandle < 0)
		{
			toonHandle = mShareToonTextureHandle[i + 1];
		}
		toonTextureHandle[i + 1] = toonHandle;
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
		unsigned short boneCount;
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
		fread(&boneCount, sizeof(boneCount), 1, fp);
		modelInfo.boneData.resize(boneCount);
		fread(modelInfo.boneData.data(), sizeof(PMDBone), boneCount, fp);


		// toon情報読み込み(仮)
		std::vector<int> toonTextureHandle;
		LoadToonData(fp, toonTextureHandle, filePath);

		fclose(fp);

		/// モデルデータの生成、登録
		auto modelData = PMDModelData::Create(mDevice, modelInfo, toonTextureHandle);

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
