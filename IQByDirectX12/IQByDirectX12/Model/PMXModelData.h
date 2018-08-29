/*
	@file		PMXModelData.h
	@brief		PMXモデルのデータを格納する
	@param		History
	2018/08/28	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <vector>

// 自作ヘッダインクルード
#include "ModelData.h"
#include "../Math/Math.h"

// クラス使用宣言
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
	/// PMXのモデルデータを生成する
	/// @note PMXModelDataはこのクラスを通じてのみ生成可能
	/// @param[in] device : ID3D12デバイス
	/// @param[in] vertexData: 頂点情報
	/// @param[in] vertIndexData: 頂点インデックス情報
	/// @retval 生成成功: PMXModelDataのスマートポインタ
	/// @retval 生成失敗時: nullptr
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
