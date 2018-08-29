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

	PMX::PMXHeader header;
	PMX::ModelInfo modelInfo;

	std::shared_ptr<PMXModelData> modelData;

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
	fread(&header, sizeof(header), 1, fp);

	// モデル情報の読み込み
	modelInfo.modelName = ReadTextBuf(fp);
	modelInfo.modelNameEng = ReadTextBuf(fp);
	modelInfo.comment = ReadTextBuf(fp);
	modelInfo.commentEng = ReadTextBuf(fp);

	// 頂点情報読み込み
	int vertsNum;

	fread(&vertsNum, sizeof(int), 1, fp);

	if (header.boneIndexSize == 1)
	{
		std::vector<PMX::Vertex<char>> verts;
		verts.resize(vertsNum);
		for (auto& vertex : verts)
		{
			fread(&vertex.position, sizeof(vertex.position), 1, fp);
			fread(&vertex.normal, sizeof(vertex.normal), 1, fp);
			fread(&vertex.uv, sizeof(vertex.uv), 1, fp);
			if (header.appendUvCount != 0)
			{
				vertex.appendUV.resize(header.appendUvCount);
				fread((void*)vertex.appendUV.data(), vertex.appendUV.size(), 1, fp);
			}
			fread(&vertex.weightDeformType, sizeof(vertex.weightDeformType), 1, fp);
			
			if (vertex.weightDeformType == 0)
			{
				fread(&vertex.bdef1, sizeof(vertex.bdef1), 1, fp);
			}
			else if (vertex.weightDeformType == 1)
			{
				fread(&vertex.bdef2, sizeof(vertex.bdef2), 1, fp);
			}
			else if (vertex.weightDeformType == 2)
			{
				fread(&vertex.bdef4, sizeof(vertex.bdef4), 1, fp);
			}
			else if (vertex.weightDeformType == 2)
			{
				fread(&vertex.sdef, sizeof(vertex.sdef), 1, fp);
			}

			fread(&vertex.edgeScale, sizeof(vertex.edgeScale), 1, fp);
		}

		// 頂点インデックス情報の読み込み
		int indexCount;
		fread(&indexCount, sizeof(int), 1, fp);
		if (header.vertexIndexSize == 1)
		{
			std::vector<PMX::Index<unsigned char>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}
		else if (header.vertexIndexSize == 2)
		{
			std::vector<PMX::Index<unsigned short>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}
		else if (header.vertexIndexSize == 4)
		{
			std::vector<PMX::Index<int>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}

	}
	else if (header.boneIndexSize == 2)
	{
		std::vector<PMX::Vertex<short>> verts;
		verts.resize(vertsNum);
		for (auto& vertex : verts)
		{
			fread(&vertex.position, sizeof(vertex.position), 1, fp);
			fread(&vertex.normal, sizeof(vertex.normal), 1, fp);
			fread(&vertex.uv, sizeof(vertex.uv), 1, fp);
			if (header.appendUvCount != 0)
			{
				vertex.appendUV.resize(header.appendUvCount);
				fread((void*)vertex.appendUV.data(), vertex.appendUV.size(), 1, fp);
			}
			fread(&vertex.weightDeformType, sizeof(vertex.weightDeformType), 1, fp);

			if (vertex.weightDeformType == 0)
			{
				fread(&vertex.bdef1, sizeof(vertex.bdef1), 1, fp);
			}
			else if (vertex.weightDeformType == 1)
			{
				fread(&vertex.bdef2, sizeof(vertex.bdef2), 1, fp);
			}
			else if (vertex.weightDeformType == 2)
			{
				fread(&vertex.bdef4, sizeof(vertex.bdef4), 1, fp);
			}
			else if (vertex.weightDeformType == 2)
			{
				fread(&vertex.sdef, sizeof(vertex.sdef), 1, fp);
			}

			fread(&vertex.edgeScale, sizeof(vertex.edgeScale), 1, fp);
		}


		// 頂点インデックス情報の読み込み
		int indexCount;
		fread(&indexCount, sizeof(int), 1, fp);
		if (header.vertexIndexSize == 1)
		{
			std::vector<PMX::Index<unsigned char>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}
		else if (header.vertexIndexSize == 2)
		{
			std::vector<PMX::Index<unsigned short>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}
		else if (header.vertexIndexSize == 4)
		{
			std::vector<PMX::Index<int>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}
	}
	else if (header.boneIndexSize == 4)
	{
		/*
		std::vector<PMX::Vertex<int>> verts;
		verts.resize(vertsNum);
		for (auto& vertex : verts)
		{
			fread(&vertex.position, sizeof(vertex.position), 1, fp);
			fread(&vertex.normal, sizeof(vertex.normal), 1, fp);
			fread(&vertex.uv, sizeof(vertex.uv), 1, fp);
			if (header.appendUvCount != 0)
			{
				vertex.appendUV.resize(header.appendUvCount);
				fread((void*)vertex.appendUV.data(), vertex.appendUV.size(), 1, fp);
			}
			fread(&vertex.weightDeformType, sizeof(vertex.weightDeformType), 1, fp);

			if (vertex.weightDeformType == 0)
			{
				fread(&vertex.bdef1, sizeof(vertex.bdef1), 1, fp);
			}
			else if (vertex.weightDeformType == 1)
			{
				fread(&vertex.bdef2, sizeof(vertex.bdef2), 1, fp);
			}
			else if (vertex.weightDeformType == 2)
			{
				fread(&vertex.bdef4, sizeof(vertex.bdef4), 1, fp);
			}
			else if (vertex.weightDeformType == 2)
			{
				fread(&vertex.sdef, sizeof(vertex.sdef), 1, fp);
			}

			fread(&vertex.edgeScale, sizeof(vertex.edgeScale), 1, fp);
		}

		// 頂点インデックス情報の読み込み
		int indexCount;
		fread(&indexCount, sizeof(int), 1, fp);
		if (header.vertexIndexSize == 1)
		{
			std::vector<PMX::Index<unsigned char>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}
		else if (header.vertexIndexSize == 2)
		{
			std::vector<PMX::Index<unsigned short>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}
		else if (header.vertexIndexSize == 4)
		{
			std::vector<PMX::Index<int>> indexies;
			indexies.resize(indexCount);
			fread((void*)indexies.data(), sizeof(indexies[0]), indexCount, fp);

			modelData = PMXModelData::Create(mDevice, verts, indexies);
		}
		*/
	}
	fclose(fp);

	return modelData;
}

std::string PMXLoader::ReadTextBuf(FILE * fp)
{
	std::string text;

	int bufSize;
	fread(&bufSize, sizeof(int), 1, fp);
	fread((void*)text.data(), sizeof(char) * bufSize, 1, fp);

	return text;
}
