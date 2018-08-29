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
	struct PMXHeader
	{
		float version;
		unsigned char byteSize;
		unsigned char encodeType;
		unsigned char appendUvCount;
		unsigned char vertexIndexSize;
		unsigned char textureIndexSize;
		unsigned char materialIndexSize;
		unsigned char boneIndexSize;
		unsigned char morphIndexSize;
		unsigned char rigidIndexSize;
	};

	struct ModelInfo
	{
		std::string modelName;
		std::string modelNameEng;
		std::string comment;
		std::string commentEng;
	};

	template<typename T>
	struct BDEF1
	{
		T boneIndex;
	};

	template<typename T>
	struct BDEF2
	{
		T boneIndex[2];
		float bone1Weight;
	};

	template<typename T>
	struct BDEF4
	{
		T boneIndex[4];
		float boneWeight[4];
	};

	template<typename T>
	struct SDEF
	{
		T boneIndex[2];
		float bone1Weight;
		Math::Vector3 c;
		Math::Vector3 r0;
		Math::Vector3 r1;
	};

	template<typename T>
	struct Vertex
	{
		Vertex() {}
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector2 uv;
		std::vector<Math::Vector4> appendUV;
		unsigned char weightDeformType;
		union
		{
			BDEF1<T> bdef1;
			BDEF2<T> bdef2;
			BDEF4<T> bdef4;
			SDEF<T> sdef;
		};
		float edgeScale;
	};


	template<typename T>
	struct Index
	{
		T vertIndex;
	};
}

class PMXModelData : ModelData
{
public:
	template<typename BoneIndexType, typename VertIndexType>
	PMXModelData(ComPtr<ID3D12Device> device,
		std::vector<PMX::Vertex<BoneIndexType>> vertexData,
		std::vector<PMX::Index<VertIndexType>> indexData);
	~PMXModelData();

	/// @fn Create
	/// PMX�̃��f���f�[�^�𐶐�����
	/// @note PMXModelData�͂��̃N���X��ʂ��Ă̂ݐ����\
	/// @param[in] device : ID3D12�f�o�C�X
	/// @param[in] vertexData: ���_���
	/// @param[in] vertIndexData: ���_�C���f�b�N�X���
	/// @retval ��������: PMXModelData�̃X�}�[�g�|�C���^
	/// @retval �������s��: nullptr
	template<typename BoneIndexType, typename VertIndexType>
	static std::shared_ptr<PMXModelData> Create(ComPtr<ID3D12Device> device, 
		std::vector<PMX::Vertex<BoneIndexType>> vertexData,
		std::vector<PMX::Index<VertIndexType>> indexData);

private:
	
};

template<typename BoneIndexType, typename VertIndexType>
inline PMXModelData::PMXModelData(ComPtr<ID3D12Device> device, std::vector<PMX::Vertex<BoneIndexType>> vertexData, std::vector<PMX::Index<VertIndexType>> indexData)
	: ModelData(VertexBuffer::Create(device, (void*)vertexData.data(), vertexData.size(),sizeof(PMX::Vertex<BoneIndexType>)), IndexBuffer::Create(device, (void*)indexData.data(), indexData.size(), sizeof(PMX::Index<VertIndexType>)) )
{
}

template<typename BoneIndexType, typename VertIndexType>
inline std::shared_ptr<PMXModelData> PMXModelData::Create(ComPtr<ID3D12Device> device, std::vector<PMX::Vertex<BoneIndexType>> vertexData, std::vector<PMX::Index<VertIndexType>> indexData)
{
	auto model = std::shared_ptr<PMXModelData>( new PMXModelData(device, vertexData, indexData));
	if (model->mVertexBuffer == nullptr || model->mIndexBuffer == nullptr)
	{
		return nullptr;
	}
	return model;
}
