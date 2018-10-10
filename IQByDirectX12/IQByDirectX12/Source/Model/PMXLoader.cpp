#include "PMXLoader.h"
#include "../ConvertString.h"
#include "../Debug/DebugLayer.h"
#include <cstdio>
#include <iostream>


PMXLoader::PMXLoader(std::shared_ptr<Device> device)
	: ModelLoader(device)
{
}


PMXLoader::~PMXLoader()
{
}

std::shared_ptr<PMXLoader> PMXLoader::Create(std::shared_ptr<Device> device)
{
	auto modelLoader = std::shared_ptr<PMXLoader>(new PMXLoader(device));
	return modelLoader;
}

std::shared_ptr<Model> PMXLoader::LoadModel(const std::string & filePath)
{
	auto handleItr = mModelHandleManager.find(filePath);

	if (handleItr == mModelHandleManager.end() || !mModelDataManager.IsExist((*handleItr).second))
	{

		FILE *fp;
		PMX::ModelDataDesc modelDataDesc;

		auto err = fopen_s(&fp, filePath.c_str(), "rb");
		if (err != 0)
		{
			std::string str = "Failed File Open \"" + filePath + "\n";
			DebugLayer::GetInstance().PrintDebugMessage(str);
			return nullptr;
		}

		std::string fileSignature;
		fileSignature.clear();
		fileSignature.resize(SIGNATURE_SIZE);
		fread((void*)fileSignature.data(), SIGNATURE_SIZE, 1, fp);
		if (fileSignature != FILE_SIGNATURE)
		{
			std::string str = "This file is not PMX. Prease load \".pmx\" file.";
			DebugLayer::GetInstance().PrintDebugMessage(str);
			fclose(fp);
			return nullptr;
		}

		modelDataDesc.modelFilePath = ConvertStringToWString(filePath);

		// ヘッダデータ読み込み
		LoadHeader(modelDataDesc.header, fp);

		// モデル情報の読み込み
		LoadModelInfo(modelDataDesc.modelInfo, fp);

		// 頂点情報読み込み
		LoadVertexData(modelDataDesc.vertices, modelDataDesc.header, fp);

		// インデックス情報読み込み
		LoadIndexData(modelDataDesc.indexies, modelDataDesc.header, fp);

		// テクスチャ情報読み込み
		LoadTextureData(modelDataDesc.textures, fp);

		// マテリアル情報読み込み
		LoadMaterial(modelDataDesc.materials, modelDataDesc.header, fp);

		// ボーン情報読み込み
		LoadBone(modelDataDesc.bones, static_cast<size_t>( modelDataDesc.header.pmxDataInfo[static_cast<int>(PMX::DataInfo::boneIndexSize)] ), fp);

		// モーフ情報読み込み
		LoadMorph(modelDataDesc.morphs, modelDataDesc.header, fp);

		// 表示枠読み込み
		LoadDisplayFrame(modelDataDesc.displayFrame, static_cast<size_t>(modelDataDesc.header.pmxDataInfo[static_cast<int>(PMX::DataInfo::boneIndexSize)]), static_cast<size_t>(modelDataDesc.header.pmxDataInfo[static_cast<int>(PMX::DataInfo::morphIndexSize)]), fp);

		fclose(fp);

		auto modelData = PMXModelData::Create(mDevice, modelDataDesc);
		mModelHandleManager[filePath] = mModelDataManager.Regist(modelData);
	}
	return std::shared_ptr<Model>(new Model(mModelHandleManager[filePath]));
}

void PMXLoader::ClearModelData()
{
}

std::string PMXLoader::ReadTextBuf(FILE * fp)
{
	std::string text;

	int bufSize;
	fread(&bufSize, sizeof(int), 1, fp);
	text.resize(bufSize);
	fread((void*)text.data(), sizeof(char) * bufSize, 1, fp);

	return text;
}

std::wstring PMXLoader::ReadTextBufWString(FILE * fp)
{
	std::wstring text;

	int bufSize;
	fread(&bufSize, sizeof(int), 1, fp);
	bufSize = bufSize / sizeof(wchar_t);
	text.resize(bufSize + 1);
	fread((void*)text.data(), sizeof(wchar_t)*bufSize, 1, fp);
	text[bufSize] = L'\0';
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
	modelInfo.modelName = ReadTextBufWString(fp);
	modelInfo.modelNameEng = ReadTextBufWString(fp);
	modelInfo.comment = ReadTextBufWString(fp);
	modelInfo.commentEng = ReadTextBufWString(fp);
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
			for (int i = 0; i < header.pmxDataInfo[(int)PMX::DataInfo::appendUVCount]; ++i)
			{
				fread(&vertex.appendUV[i], sizeof(vertex.appendUV[i]), 1, fp);
			}
		}

		fread(&vertex.weightDeformType, sizeof(char), 1, fp);
		
		switch (vertex.weightDeformType)
		{
		case PMX::WeightDeformType::BDEF1:
			fread(&vertex.deformParam.boneIndex[0], boneIndexSize, 1, fp);
			break;

		case PMX::WeightDeformType::BDEF2:
			fread(&vertex.deformParam.boneIndex[0], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneIndex[1], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneWeight[0], sizeof(vertex.deformParam.boneWeight[0]), 1, fp);
			break;

		case PMX::WeightDeformType::BDEF4:
			fread(&vertex.deformParam.boneIndex[0], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneIndex[1], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneIndex[2], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneIndex[3], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneWeight[0], sizeof(vertex.deformParam.boneWeight[0]), 1, fp);
			fread(&vertex.deformParam.boneWeight[1], sizeof(vertex.deformParam.boneWeight[1]), 1, fp);
			fread(&vertex.deformParam.boneWeight[2], sizeof(vertex.deformParam.boneWeight[2]), 1, fp);
			fread(&vertex.deformParam.boneWeight[3], sizeof(vertex.deformParam.boneWeight[3]), 1, fp);
			break;

		case PMX::WeightDeformType::SDEF:
			fread(&vertex.deformParam.boneIndex[0], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneIndex[1], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneWeight[0], sizeof(vertex.deformParam.boneWeight[0]), 1, fp);
			fread(&vertex.deformParam.c, sizeof(vertex.deformParam.c), 1, fp);
			fread(&vertex.deformParam.r0, sizeof(vertex.deformParam.r0), 1, fp);
			fread(&vertex.deformParam.r1, sizeof(vertex.deformParam.r1), 1, fp);
			break;
		case PMX::WeightDeformType::QDEF:
			fread(&vertex.deformParam.boneIndex[0], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneIndex[1], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneIndex[2], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneIndex[3], boneIndexSize, 1, fp);
			fread(&vertex.deformParam.boneWeight[0], sizeof(vertex.deformParam.boneWeight[0]), 1, fp);
			fread(&vertex.deformParam.boneWeight[1], sizeof(vertex.deformParam.boneWeight[1]), 1, fp);
			fread(&vertex.deformParam.boneWeight[2], sizeof(vertex.deformParam.boneWeight[2]), 1, fp);
			fread(&vertex.deformParam.boneWeight[3], sizeof(vertex.deformParam.boneWeight[3]), 1, fp);
			break;
		default:
			DebugLayer::GetInstance().PrintDebugMessage("PMX File Load Error.\n");
			return;
		}
		fread(&vertex.edgeScale, sizeof(vertex.edgeScale), 1, fp);
	}
}

void PMXLoader::LoadIndexData(std::vector<PMX::Index>& indexData, const PMX::Header & header, FILE * fp)
{
	int indexCount;
	fread(&indexCount, sizeof(indexCount), 1, fp);
	indexData.resize(indexCount);
	size_t vertIndexSize = header.pmxDataInfo[(int)PMX::DataInfo::vertexIndexSize];
	
	for (auto& index : indexData)
	{
		fread(&index.vertIndex, vertIndexSize, 1, fp);
	}
}

void PMXLoader::LoadTextureData(std::vector<PMX::Texture>& textureData, FILE * fp)
{
	int textureNum;
	fread(&textureNum, sizeof(textureNum), 1, fp);
	textureData.resize(textureNum);
	for (auto& texture : textureData)
	{
		texture.texturePath = ReadTextBufWString(fp);
	}
}

void PMXLoader::LoadMaterial(std::vector<PMX::Material>& materialData, const PMX::Header & header, FILE * fp)
{
	int materialNum;
	int textureIndexSize = header.pmxDataInfo[(int)PMX::DataInfo::textureIndexSize];
	fread(&materialNum, sizeof(materialNum), 1, fp);
	materialData.resize(materialNum);
	for (auto& material : materialData)
	{
		material.name = ReadTextBufWString(fp);
		material.nameEng = ReadTextBufWString(fp);
		fread(&material.diffuse, sizeof(material.diffuse), 1, fp);
		fread(&material.specular, sizeof(material.specular), 1, fp);
		fread(&material.specularFactor, sizeof(material.specularFactor), 1, fp);
		fread(&material.ambient, sizeof(material.ambient), 1, fp);
		fread(&material.drawFlags, sizeof(material.drawFlags), 1, fp);
		fread(&material.edgeColor, sizeof(material.edgeColor), 1, fp);
		fread(&material.edgeSize, sizeof(material.edgeSize), 1, fp);
		fread(&material.textureIndex, textureIndexSize, 1, fp);
		fread(&material.sphereTextureIndex, textureIndexSize, 1, fp);
		fread(&material.sphereMode, sizeof(material.sphereMode), 1, fp);
		fread(&material.shareToonFlag, sizeof(material.shareToonFlag), 1, fp);
		if (material.shareToonFlag == 0)
		{
			fread(&material.toonTextureIndex, textureIndexSize, 1, fp);
		}
		else if (material.shareToonFlag == 1)
		{
			fread(&material.shareToonTexture, sizeof(material.shareToonTexture), 1, fp);
		}

		material.materialMemo = ReadTextBufWString(fp);
		fread(&material.vertNum, sizeof(material.vertNum), 1, fp);
	}
}

void PMXLoader::LoadBone(std::vector<PMX::BoneData>& boneData, size_t boneIndexSize, FILE * fp)
{
	int boneCount;
	fread(&boneCount, sizeof(boneCount), 1, fp);
	boneData.resize(boneCount);
	for (auto& bone : boneData)
	{
		bone.name = ReadTextBufWString(fp);
		bone.nameEng = ReadTextBufWString(fp);

		fread(&bone.position, sizeof(bone.position), 1, fp);
		fread(&bone.parentBoneIndex, boneIndexSize, 1, fp);

		fread(&bone.deformHierarchy, sizeof(bone.deformHierarchy), 1, fp);

		fread(&bone.boneFlags, sizeof(bone.boneFlags), 1, fp);

		if (bone.accessPointFlag == 0)
		{
			fread(&bone.accessPointPosition, sizeof(bone.accessPointPosition), 1, fp);
		}
		else
		{
			fread(&bone.accessPointBoneIndex, boneIndexSize, 1, fp);
		}

		if (bone.giveRotation== 1 || bone.giveMove == 1)
		{
			fread(&bone.giveStateParentBoneIndex, boneIndexSize, 1, fp);
			fread(&bone.giveStateGrantRate, sizeof(bone.giveStateGrantRate), 1, fp);
		}

		if (bone.axisFixed == 1)
		{
			fread(&bone.axisVector, sizeof(bone.axisVector), 1, fp);
		}

		if (bone.localAxis == 1)
		{
			fread(&bone.localXAxis, sizeof(bone.localXAxis), 1, fp);
			fread(&bone.localZAxis, sizeof(bone.localZAxis), 1, fp);
		}

		if (bone.externalParentDeform == 1)
		{
			fread(&bone.externalParentDeformKey, sizeof(bone.externalParentDeformKey), 1, fp);
		}

		if (bone.ik == 1)
		{
			auto& ikData = bone.ikData;
			fread(&ikData.ikTargetBoneIndex, boneIndexSize, 1, fp);
			fread(&ikData.ikLoopCount, sizeof(ikData.ikLoopCount), 1, fp);
			fread(&ikData.ikLoopLimitAngle, sizeof(ikData.ikLoopLimitAngle), 1, fp);
			fread(&ikData.ikLinkNum, sizeof(ikData.ikLinkNum), 1, fp);
			ikData.ikLinks.resize(ikData.ikLinkNum);
			for (auto& ikLink : ikData.ikLinks)
			{
				fread(&ikLink.boneIndex, boneIndexSize, 1, fp);
				fread(&ikLink.rotateLimit, sizeof(ikLink.rotateLimit), 1, fp);
				if (ikLink.rotateLimit == 1)
				{
					fread(&ikLink.lowerLimitAngle, sizeof(ikLink.lowerLimitAngle), 1, fp);
					fread(&ikLink.upperLimitAngle, sizeof(ikLink.upperLimitAngle), 1, fp);
				}
			}
		}
	}
}

void PMXLoader::LoadMorph(std::vector<PMX::Morph>& morphData, const PMX::Header & header, FILE * fp)
{
	size_t vertexIndexSize = static_cast<size_t>(header.pmxDataInfo[static_cast<int>(PMX::DataInfo::vertexIndexSize)]);
	size_t boneIndexSize = static_cast<size_t>(header.pmxDataInfo[static_cast<int>(PMX::DataInfo::boneIndexSize)]);
	size_t materialIndexSize = static_cast<size_t>(header.pmxDataInfo[static_cast<int>(PMX::DataInfo::materialIndexSize)]);
	size_t morphIndexSize = static_cast<size_t>(header.pmxDataInfo[static_cast<int>(PMX::DataInfo::morphIndexSize)]);
	
	int morphCount;
	fread(&morphCount, sizeof(morphCount), 1, fp);
	morphData.resize(morphCount);
	for (auto& morph : morphData)
	{
		morph.name = ReadTextBufWString(fp);
		morph.nameEng = ReadTextBufWString(fp);

		fread(&morph.controlPanel, sizeof(morph.controlPanel), 1, fp);
		fread(&morph.morphType, sizeof(morph.morphType), 1, fp);

		fread(&morph.morphDataCount, sizeof(morph.morphDataCount), 1, fp);
		
		morph.morphData.resize(morph.morphDataCount);
		for (auto& morphInfo : morph.morphData)
		{
			switch (static_cast<PMX::MorphType>(morph.morphType))
			{
			case PMX::MorphType::Groupe:
				fread(&morphInfo.groupeMorph.morphIndex, morphIndexSize, 1, fp);
				fread(&morphInfo.groupeMorph.morphRate, sizeof(morphInfo.groupeMorph.morphRate), 1, fp);
				break;
			case PMX::MorphType::Vertex:
				fread(&morphInfo.vertexMorph.vertIndex, vertexIndexSize, 1, fp);
				fread(&morphInfo.vertexMorph.positionOffset, sizeof(morphInfo.vertexMorph.positionOffset), 1, fp);
				break;
			case PMX::MorphType::Bone:
				fread(&morphInfo.boneMorph.boneIndex, boneIndexSize, 1, fp);
				fread(&morphInfo.boneMorph.moveOffset, sizeof(morphInfo.boneMorph.moveOffset), 1, fp);
				fread(&morphInfo.boneMorph.rotation, sizeof(morphInfo.boneMorph.rotation), 1, fp);
				break;
			case PMX::MorphType::UV:
			case PMX::MorphType::AppendUV1:
			case PMX::MorphType::AppendUV2:
			case PMX::MorphType::AppendUV3:
			case PMX::MorphType::AppendUV4:
				fread(&morphInfo.uvMorph.vertIndex, vertexIndexSize, 1, fp);
				fread(&morphInfo.uvMorph.uvOffset, sizeof(morphInfo.uvMorph.uvOffset), 1, fp);
				break;
			case PMX::MorphType::Material:
				fread(&morphInfo.materialMorph.materialIndex, materialIndexSize, 1, fp);
				fread(&morphInfo.materialMorph.offsetCalcType, sizeof(morphInfo.materialMorph.offsetCalcType), 1, fp);
				fread(&morphInfo.materialMorph.diffuse, sizeof(morphInfo.materialMorph.diffuse), 1, fp);
				fread(&morphInfo.materialMorph.specular, sizeof(morphInfo.materialMorph.specular), 1, fp);
				fread(&morphInfo.materialMorph.specularity, sizeof(morphInfo.materialMorph.specularity), 1, fp);
				fread(&morphInfo.materialMorph.ambient, sizeof(morphInfo.materialMorph.ambient), 1, fp);
				fread(&morphInfo.materialMorph.edgeColor, sizeof(morphInfo.materialMorph.edgeColor), 1, fp);
				fread(&morphInfo.materialMorph.edgeSize, sizeof(morphInfo.materialMorph.edgeSize), 1, fp);
				fread(&morphInfo.materialMorph.textureFactor, sizeof(morphInfo.materialMorph.textureFactor), 1, fp);
				fread(&morphInfo.materialMorph.sphereTextureFactor, sizeof(morphInfo.materialMorph.sphereTextureFactor), 1, fp);
				fread(&morphInfo.materialMorph.toonTextureFactor, sizeof(morphInfo.materialMorph.toonTextureFactor), 1, fp);
				break;
			default:
				break;
			}
		}
	}
}

void PMXLoader::LoadDisplayFrame(std::vector<PMX::DisplayFrame>& displayFrameData, size_t boneIndexSize, size_t morphIndexSize, FILE * fp)
{
	int displayFrameCount;
	fread(&displayFrameCount, sizeof(displayFrameCount), 1, fp);
	displayFrameData.resize(displayFrameCount);
	for (auto& displayFrame : displayFrameData)
	{
		displayFrame.name = ReadTextBufWString(fp);
		displayFrame.nameEng = ReadTextBufWString(fp);

		fread(&displayFrame.specialFrameFlag, sizeof(displayFrame.specialFrameFlag), 1, fp);
		fread(&displayFrame.frameElementCount, sizeof(displayFrame.frameElementCount), 1, fp);
		displayFrame.frameElements.resize(displayFrame.frameElementCount);
		for (auto displayFrameElement : displayFrame.frameElements)
		{
			fread(&displayFrameElement.elementType, sizeof(displayFrameElement.elementType), 1, fp);
			if (displayFrameElement.elementType == 0)
			{
				fread(&displayFrameElement.boneIndex, boneIndexSize, 1, fp);
			}
			else if (displayFrameElement.elementType == 1)
			{
				fread(&displayFrameElement.morphIndex, morphIndexSize, 1, fp);
			}
		}
	}
}
