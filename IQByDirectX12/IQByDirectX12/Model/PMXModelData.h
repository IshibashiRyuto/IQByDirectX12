/*
	@file		PMXModelData.h
	@brief		PMX���f���̃f�[�^���i�[����
	@param		History
	2018/08/28	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <vector>

// ����w�b�_�C���N���[�h
#include "ModelData.h"
#include "../Math/Math.h"

// �N���X�g�p�錾
class VertexBuffer;
class IndexBuffer;


namespace PMX
{
	struct Header
	{
		float version;
		unsigned char byteSize;
		std::vector<unsigned char> pmxDataInfo;
	};

	enum class DataInfo
	{
		encodeType,
		appendUVCount,
		vertexIndexSize,
		textureIndexSize,
		materialIndexSize,
		boneIndexSize,
		morphIndexSize,
		rigidIndexSize,
	};

	struct ModelInfo
	{
		std::string modelName;
		std::string modelNameEng;
		std::string comment;
		std::string commentEng;
	};

	struct BDEF1
	{
		int boneIndex;
	};

	struct BDEF2
	{
		int boneIndex[2];
		float boneWeight;
	};

	struct BDEF4
	{
		int boneIndex[4];
		float boneWeight[4];
	};

	struct SDEF
	{
		int boneIndex[2];
		float boneWeight;
		Math::Vector3 c;
		Math::Vector3 r0;
		Math::Vector3 r1;
	};

	enum class WeightDeformType
	{
		BDEF1,
		BDEF2,
		BDEF4,
		SDEF,
	};

	union BoneDeform
	{
		BDEF1 bdef1;
		BDEF2 bdef2;
		BDEF4 bdef4;
		SDEF sdef;
	};

	struct Vertex
	{
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector2 uv;
		std::vector<Math::Vector4> appendUV;
		WeightDeformType weightDeformType;
		BoneDeform boneDeform;
		float edgeScale;
	};


	struct Index
	{
		int vertIndex;
	};
}

class PMXModelData : ModelData
{
public:
	PMXModelData(ComPtr<ID3D12Device> device,
		std::vector<PMX::Vertex> vertexData,
		std::vector<PMX::Index> indexData);
	~PMXModelData();

	/// @fn Create
	/// PMX�̃��f���f�[�^�𐶐�����
	/// @note PMXModelData�͂��̃N���X��ʂ��Ă̂ݐ����\
	/// @param[in] device : ID3D12�f�o�C�X
	/// @param[in] vertexData: ���_���
	/// @param[in] vertIndexData: ���_�C���f�b�N�X���
	/// @retval ��������: PMXModelData�̃X�}�[�g�|�C���^
	/// @retval �������s��: nullptr
	static std::shared_ptr<PMXModelData> Create(ComPtr<ID3D12Device> device, 
		std::vector<PMX::Vertex> vertexData,
		std::vector<PMX::Index> indexData);

private:
	
};