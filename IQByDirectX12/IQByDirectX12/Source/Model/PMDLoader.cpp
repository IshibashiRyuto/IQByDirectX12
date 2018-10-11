#include <iostream>
#include "PMDLoader.h"


PMDLoader::PMDLoader(std::shared_ptr<Device> device)
	: ModelLoader(device)
{
}


PMDLoader::~PMDLoader()
{
	ClearModelData();
}

std::shared_ptr<PMDLoader> PMDLoader::Create(std::shared_ptr<Device> device)
{
	auto pmdLoader = std::shared_ptr<PMDLoader>(new PMDLoader(device));
	pmdLoader->mDevice = device;
	return pmdLoader;
}

std::shared_ptr<Model> PMDLoader::LoadModel(const std::string & filePath)
{
	auto handleItr = mModelHandleManager.find(filePath);
	

	// �t�@�C���p�X�ɑΉ����郂�f���n���h�������݂��Ȃ�
	// or ���f���n���h���ɑΉ����郂�f���f�[�^�����ł��Ă���
	if (handleItr == mModelHandleManager.end() || !mModelDataManager.IsExist((*handleItr).second))
	{
		// ���f���ǂݍ���

		PMDModelInfo modelInfo;

		unsigned int vertexCount;
		unsigned int indexCount;
		unsigned int materialCount;
		PMDHeader pmdHeader;

		// �t�@�C�����J��
		FILE *fp;
		fopen_s(&fp, filePath.c_str(), "rb");

		if (fp == nullptr)
		{
			return nullptr;
		}

		// �t�@�C���t�H�[�}�b�g�̊m�F
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

		// �w�b�_�f�[�^�ǂݍ���
		fread(&pmdHeader, sizeof(pmdHeader), 1, fp);

		// ���_���̓ǂݍ���
		fread(&vertexCount, sizeof(int), 1, fp);
		modelInfo.vertexData.resize(vertexCount);
		fread(modelInfo.vertexData.data(), sizeof(PMDVertex), vertexCount, fp);

		// ���_�C���f�b�N�X���ǂݍ���
		fread(&indexCount, sizeof(int), 1, fp);
		modelInfo.indexData.resize(indexCount);
		fread(modelInfo.indexData.data(), sizeof(short), indexCount, fp);

		// �}�e���A���p�����[�^�ǂݍ���
		fread(&materialCount, sizeof(int), 1, fp);
		modelInfo.materials.resize(materialCount);
		fread(modelInfo.materials.data(), sizeof(PMDMaterial), materialCount, fp);

		fclose(fp);

		/// ���f���f�[�^�̐����A�o�^
		auto modelData = PMDModelData::Create(mDevice, modelInfo);

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
