#include "PMXLoader.h"
#include <cstdio>
#include <iostream>

PMXLoader::PMXLoader()
{
}


PMXLoader::~PMXLoader()
{
}

std::shared_ptr<PMXLoader> PMXLoader::Create(ComPtr<ID3D12Device> device)
{
	auto modelLoader = std::shared_ptr<PMXLoader>(new PMXLoader());
	modelLoader->mDevice = device;
	return modelLoader;
}

std::shared_ptr<PMXModelData> PMXLoader::LoadModel(const std::string & filePath)
{
	FILE *fp;

	PMX::Header header;
	PMX::ModelInfo modelInfo;
	std::vector<PMX::Vertex> vertices;
	std::vector<PMX::Index> indexies;

	auto err = fopen_s(&fp, filePath.c_str(), "rb");
	if (err != 0)
	{
#ifdef _DEBUG
		std::cout << "Failed File Open \"" << filePath << std::endl;
#endif
		return nullptr;
	}

	std::string fileSignature;
	fread((void*)fileSignature.data(), SIGNATURE_SIZE, 1, fp);
	if (fileSignature != FILE_SIGNATURE)
	{
#ifdef _DEBUG
		std::cout << "This file is not PMX. Prease load \".pmx\" file." << std::endl;
#endif
		fclose(fp);
		return nullptr;
	}

	// ヘッダデータ読み込み
	LoadHeader(header, fp);

	// モデル情報の読み込み
	LoadModelInfo(modelInfo, fp);

	// 頂点情報読み込み
	LoadVertexData(vertices, header, fp);

	// インデックス情報読み込み
	LoadIndexData(indexies, header, fp);

	fclose(fp);

	return nullptr;
}

std::string PMXLoader::ReadTextBuf(FILE * fp)
{
	std::string text;

	int bufSize;
	fread(&bufSize, sizeof(int), 1, fp);
	fread((void*)text.data(), sizeof(char) * bufSize, 1, fp);

	return text;
}

void PMXLoader::LoadHeader(PMX::Header & header, FILE * fp)
{
	fread(&header.version, sizeof(header.version), 1, fp);
	fread(&header.byteSize, sizeof(header.byteSize), 1, fp);

	if (header.byteSize <= 0)
	{
		return;
	}

	header.pmxDataInfo.resize(header.byteSize);
	fread((void*)header.pmxDataInfo.data(), sizeof(header.pmxDataInfo[0]), header.pmxDataInfo.size(), fp);
}

void PMXLoader::LoadModelInfo(PMX::ModelInfo & modelInfo, FILE * fp)
{
	modelInfo.modelName = ReadTextBuf(fp);
	modelInfo.modelNameEng = ReadTextBuf(fp);
	modelInfo.comment = ReadTextBuf(fp);
	modelInfo.commentEng = ReadTextBuf(fp);
}

void PMXLoader::LoadVertexData(std::vector<PMX::Vertex>& vertexData, const PMX::Header & header, FILE * fp)
{
	int vertexCount;
	bool isUseAppendUV = (header.pmxDataInfo[(int)PMX::DataInfo::appendUVCount] > 0);
	size_t boneIndexSize = header.pmxDataInfo[(int)PMX::DataInfo::boneIndexSize];

	fread(&vertexCount, sizeof(vertexCount), 1, fp);
	vertexData.resize(vertexCount);

	for (auto& vertex : vertexData)
	{
		fread(&vertex.position, sizeof(vertex.position), 1, fp);
		fread(&vertex.normal, sizeof(vertex.normal), 1, fp);
		fread(&vertex.uv, sizeof(vertex.uv), 1, fp);
		
		if (isUseAppendUV)
		{
			vertex.appendUV.resize(header.pmxDataInfo[(int)PMX::DataInfo::appendUVCount]);
			fread((void*)vertex.appendUV.data(), sizeof(vertex.appendUV[0]), vertex.appendUV.size(), fp);
		}

		fread(&vertex.weightDeformType, sizeof(char), 1, fp);
		
		switch (vertex.weightDeformType)
		{
		case PMX::WeightDeformType::BDEF1:
			fread(&vertex.boneDeform.bdef1, boneIndexSize, 1, fp);
			break;

		case PMX::WeightDeformType::BDEF2:
			fread(&vertex.boneDeform.bdef2.boneIndex, boneIndexSize, 2, fp);
			fread(&vertex.boneDeform.bdef2.boneWeight, sizeof(vertex.boneDeform.bdef2.boneWeight), 1, fp);
			break;

		case PMX::WeightDeformType::BDEF4:
			fread(&vertex.boneDeform.bdef4.boneIndex, boneIndexSize, 4, fp);
			fread(&vertex.boneDeform.bdef4.boneWeight, sizeof(vertex.boneDeform.bdef2.boneWeight), 4, fp);
			break;

		case PMX::WeightDeformType::SDEF:
			fread(&vertex.boneDeform.sdef.boneIndex, boneIndexSize, 2, fp);
			fread(&vertex.boneDeform.sdef.boneWeight, sizeof(vertex.boneDeform.sdef.boneWeight), 1, fp);
			fread(&vertex.boneDeform.sdef.c, sizeof(vertex.boneDeform.sdef.c), 1, fp);
			fread(&vertex.boneDeform.sdef.r0, sizeof(vertex.boneDeform.sdef.r0), 1, fp);
			fread(&vertex.boneDeform.sdef.r1, sizeof(vertex.boneDeform.sdef.r1), 1, fp);
			break;
		default:
#ifdef _DEBUG
			std::cout << "PMX File Load Error." << std::endl;
#endif
			return;
			break;
		}
		fread(&vertex.edgeScale, sizeof(vertex.edgeScale), 1, fp);
	}
}

void PMXLoader::LoadIndexData(std::vector<PMX::Index>& indexData, const PMX::Header & header, FILE * fp)
{
	int indexCount;
	fread(&indexCount, sizeof(indexCount), 1, fp);
	indexData.resize(indexCount);
	fread((void*)indexData.data(), header.pmxDataInfo[(int)PMX::DataInfo::vertexIndexSize], indexData.size(), fp);
}
