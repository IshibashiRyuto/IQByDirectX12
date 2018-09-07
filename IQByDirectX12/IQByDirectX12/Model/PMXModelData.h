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
#include <wrl.h>
#include <d3d12.h>

// 自作ヘッダインクルード
#include "ModelData.h"
#include "../Math/Math.h"

// クラス使用宣言
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
using Microsoft::WRL::ComPtr;

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

	struct Position
	{
		float x;
		float y;
		float z;
	};

	struct SDEF
	{
		int boneIndex[2];
		float boneWeight;
		Position c;
		Position r0;
		Position r1;
	};

	enum class WeightDeformType
	{
		BDEF1,
		BDEF2,
		BDEF4,
		SDEF,
	};

	struct Vertex
	{
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector2 uv;
		std::vector<Math::Vector4> appendUV;
		WeightDeformType weightDeformType;
		union
		{
			BDEF1 bdef1;
			BDEF2 bdef2;
			BDEF4 bdef4;
			SDEF sdef;
		};

		float edgeScale;
	};


	struct Index
	{
		int vertIndex;
	};

	struct Texture
	{
		std::string texturePath;
	};

	struct Material
	{
		std::string name;
		std::string nameEng;
		Math::Vector4 diffuse;
		Math::Vector3 specular;
		float specularFactor;
		Math::Vector3 ambient;
		union
		{
			unsigned char drawFlags;
			struct 
			{
				unsigned char doubleSideDraw	: 1;
				unsigned char groundShadow		: 1;
				unsigned char selfShadowMapDraw : 1;
				unsigned char selfShadow		: 1;
				unsigned char edgeDraw			: 1;
			};
		};
		Math::Vector4 edgeColor;
		float edgeSize;
		int textureIndex;
		int sphereTextureIndex;
		unsigned char sphereMode;
		unsigned char shareToonFlag;
		union
		{
			unsigned char shareToonTexture;
			int toonTextureIndex;
		};

		std::string materialMemo;
		int vertNum;
	};

	enum class SphereMode
	{
		Invalid,
		Mul,
		Add,
		SubTexture
	};

	struct IKLink
	{
		int boneIndex;
		unsigned char rotateLimit;
		Math::Vector3 lowerLimitAngle;
		Math::Vector3 upperLimitAngle;
	};

	struct IKData
	{
		int ikTargetBoneIndex;
		int ikLoopCount;
		float ikLoopLimitAngle;
		int ikLinkNum;
		std::vector<IKLink> ikLinks;
	};

	struct Bone
	{
		std::string name;
		std::string nameEng;
		Math::Vector3 position;
		int parentBoneIndex;
		int deformHierarchy;
		union
		{
			short boneFlags;
			struct
			{
				unsigned char accessPointFlag : 1;
				unsigned char rotatable : 1;
				unsigned char movable : 1;
				unsigned char display : 1;
				unsigned char operational : 1;
				unsigned char ik : 1;
				unsigned char local : 1;
				unsigned char giveRotation : 1;
				unsigned char axisFixed : 1;
				unsigned char localAxis : 1;
				unsigned char physicsPostDeform : 1;
				unsigned char externalParentDeform : 1;
			};
		};
		union AccessPoint
		{
			float position[3];
			int boneIndex;
		};

		struct GiveState
		{
			int parentBoneIndex;
			float grantRate;
		};
		Math::Vector3 axisVector;
		Math::Vector3 localXAxis;
		Math::Vector3 localZAxis;
		int externalParentDeformKey;
		IKData ikData;
	};

	struct VertexMorph
	{
		int vertIndex;
		float positionOffset[3];
	};

	struct UVMorph
	{
		int vertIndex;
		float uvOffset[4];
	};

	struct BoneMorph
	{
		int boneIndex;
		float moveOffset[3];
		float rotation[4];
	};

	struct MaterialMorph
	{
		int materialIndex;
		unsigned char offsetCalcType : 1;
		float diffuse[4];
		float specular[3];
		float specularFactor;
		float ambient[3];
		float edgeColor[4];
		float edgeSize;
		float textureFactor[4];
		float sphereTextureFactor[4];
		float ToonTextureFactor[4];
	};

	struct GroupeMorph
	{
		int morphIndex;
		float morphRate;
	};

	union MorphData
	{
		VertexMorph vertexMorph;
		UVMorph uvMorph;
		BoneMorph boneMorph;
		MaterialMorph materialMorph;
		GroupeMorph groupeMorph;
	};

	struct Morph
	{
		std::string name;
		std::string nameEng;
		unsigned char controlPanel;
		unsigned char morphType;
		int morphDataCount;
		std::vector<MorphData> morphData;
	};

	enum class MorphType
	{
		Groupe,
		Vertex,
		Bone,
		UV,
		AppendUV1,
		AppendUV2,
		AppendUV3,
		AppendUV4,
		Material,
	};

	struct FrameData
	{
		unsigned char dataType;
		union
		{
			int boneIndex;
			int morphIndex;
		};
	};

	struct DisplayFrame
	{
		std::string name;
		std::string nameEng;
		unsigned char specialFrameFlag;
		int FrameDataCount;
		std::vector<FrameData> frameData;
	};

	struct RigitBody
	{
		std::string name;
		std::string nameEng;
		int boneIndex;
		unsigned char groupe;
		unsigned short notCorrisionGroupeFlag;
		unsigned char shape;
		Math::Vector3 size;
		Math::Vector3 position;
		Math::Vector3 rotation;
		float mass;
		float moveDecay;
		float rotDecay;
		float reflection;
		float fliction;
		unsigned char physicsFlag;
	};
	
	struct NormalJoint
	{
		int rigidBodyAIndex;
		int rigidBodyBIndex;
		Math::Vector3 position;
		Math::Vector3 rotation;

		Math::Vector3 moveLowerLimit;
		Math::Vector3 moveUpperLimit;
		Math::Vector3 spineLowerLimit;
		Math::Vector3 spineUpperLimit;

		Math::Vector3 moveSpringFactor;
		Math::Vector3 spineSpringFactor;
	};

	struct Joint
	{
		std::string name;
		std::string nameEng;
		unsigned char jointType;
		NormalJoint jointData;
	};

	struct ModelDataDesc
	{
		Header header;
		ModelInfo modelInfo;
		std::vector<Vertex> vertices;
		std::vector<Index> indexies;
		std::vector<PMX::Texture> textures;
		std::vector<Material> materials;
		std::vector<Bone> bones;
		std::vector<Morph> morphs;
		std::vector<DisplayFrame> displayFrame;
		std::vector<RigitBody> rigidBodies;
		std::vector<Joint> joints;
	};


	struct MaterialData
	{
		Math::Vector4 diffuse;
		Math::Vector3 specular;
		float specularity;
		Math::Vector3 ambient;
		int vertsNum;
	};
}


class PMXModelData : public ModelData
{
public:
	PMXModelData(ComPtr<ID3D12Device> device,
		std::vector<PMX::Vertex> vertexData,
		std::vector<PMX::Index> indexData,
		int materialCount);
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
	static std::shared_ptr<PMXModelData> Create(ComPtr<ID3D12Device> device, const PMX::ModelDataDesc& modelDataDesc);

	/// @fn Draw
	/// 描画処理
	/// @param[in]	graphicsCommandList	: コマンドリスト
	/// @param[in]	instanceData		: インスタンスデータ
	void Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;

private:
	/* 定数定義 */

	/* 変数宣言 */
	std::shared_ptr<ConstantBuffer> mMaterialDataBuffer;			// マテリアルデータを保存する定数バッファ
	std::vector<PMX::MaterialData> mMaterialData;

	/* ローカルメソッド定義 */
	void SetMaterial(const std::vector<PMX::Material>& materials);
};