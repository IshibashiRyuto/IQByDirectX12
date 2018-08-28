#include <iostream>
#include "PMDLoader.h"


PMDLoader::PMDLoader()
{
}


PMDLoader::~PMDLoader()
{
}

std::shared_ptr<PMDLoader> PMDLoader::Create(ComPtr<ID3D12Device> device)
{
	auto pmdLoader = std::shared_ptr<PMDLoader>(new PMDLoader());
	pmdLoader->mDevice = device;
	return pmdLoader;
}

std::shared_ptr<PMDModelData> PMDLoader::LoadModel(const std::string & filePath)
{
	unsigned int vertexCount;
	std::vector<PMDVertex> vertex;
	unsigned int indexCount;
	std::vector<unsigned short> index;
	unsigned int materialCount;
	std::vector<PMDMaterial> materials;

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
		fread(pmdSignature, sizeof(char)*3, 1, fp);
		pmdSignature[3] = '\0';
		if (pmdSignature == nullptr || strcmp(pmdSignature, "Pmd") != 0)
		{
			std::cout << "This file is not pmd.\n Please load \".pmd\" file." << std::endl;
			fclose(fp);
			return nullptr;
		}
	}

	// �w�b�_�f�[�^�ǂݍ���
	fread(&pmdHeader, sizeof(pmdHeader), 1, fp);

	// ���_���̓ǂݍ���
	fread(&vertexCount, sizeof(int), 1, fp);
	vertex.resize(vertexCount);
	fread(vertex.data(), sizeof(PMDVertex), vertex.size(), fp);

	// ���_�C���f�b�N�X���ǂݍ���
	fread(&indexCount, sizeof(int), 1, fp);
	index.resize(indexCount);
	fread(index.data(), sizeof(short), index.size(), fp);

	// �}�e���A���p�����[�^�ǂݍ���
	fread(&materialCount, sizeof(int), 1, fp);
	materials.resize(materialCount);
	fread(materials.data(), sizeof(PMDMaterial), materials.size(), fp);

	fclose(fp);

	return PMDModelData::Create(mDevice, vertex, index, materials);
}
