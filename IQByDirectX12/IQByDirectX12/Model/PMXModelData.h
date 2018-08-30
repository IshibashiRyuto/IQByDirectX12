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
	/// PMXのモデルデータを生成する
	/// @note PMXModelDataはこのクラスを通じてのみ生成可能
	/// @param[in] device : ID3D12デバイス
	/// @param[in] vertexData: 頂点情報
	/// @param[in] vertIndexData: 頂点インデックス情報
	/// @retval 生成成功: PMXModelDataのスマートポインタ
	/// @retval 生成失敗時: nullptr
	static std::shared_ptr<PMXModelData> Create(ComPtr<ID3D12Device> device, 
		std::vector<PMX::Vertex> vertexData,
		std::vector<PMX::Index> indexData);

private:
	
};