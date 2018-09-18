#pragma once
#include <vector>
#include "../Math/Math.h"

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
		std::wstring modelName;
		std::wstring modelNameEng;
		std::wstring comment;
		std::wstring commentEng;
	};


	struct DeformParam
	{
		int boneIndex[4];
		float boneWeight[4];
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
		QDEF,
	};

	struct Vertex
	{
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector2 uv;
		Math::Vector4 appendUV[4];
		WeightDeformType weightDeformType;
		DeformParam deformParam;

		float edgeScale;
	};


	struct Index
	{
		int vertIndex;
	};

	struct Texture
	{
		std::wstring texturePath;
	};

	struct Material
	{
		std::wstring name;
		std::wstring nameEng;
		Math::Vector4 diffuse;
		Math::Vector3 specular;
		float specularFactor;
		Math::Vector3 ambient;
		union
		{
			unsigned char drawFlags;
			struct
			{
				unsigned char doubleSideDraw : 1;
				unsigned char groundShadow : 1;
				unsigned char selfShadowMapDraw : 1;
				unsigned char selfShadow : 1;
				unsigned char edgeDraw : 1;
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

		std::wstring materialMemo;
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

	struct BoneData
	{
		std::wstring name;
		std::wstring nameEng;
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
				unsigned char padding1 : 1;
				unsigned char giveLocal : 1;
				unsigned char giveRotation : 1;
				unsigned char giveMove : 1;
				unsigned char axisFixed : 1;
				unsigned char localAxis : 1;
				unsigned char physicsPostDeform : 1;
				unsigned char externalParentDeform : 1;
				unsigned char padding2 : 2;
			};
		};

		Math::Vector3 accessPointPosition;
		int accessPointBoneIndex;

		int giveStateParentBoneIndex;
		float giveStateGrantRate;
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
		unsigned char offsetCalcType;
		float diffuse[4];
		float specular[3];
		float specularity;
		float ambient[3];
		float edgeColor[4];
		float edgeSize;
		float textureFactor[4];
		float sphereTextureFactor[4];
		float toonTextureFactor[4];
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
		std::wstring name;
		std::wstring nameEng;
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

	struct FrameElement
	{
		unsigned char elementType;
		union
		{
			int boneIndex;
			int morphIndex;
		};
	};

	struct DisplayFrame
	{
		std::wstring name;
		std::wstring nameEng;
		unsigned char specialFrameFlag;
		int frameElementCount;
		std::vector<FrameElement> frameElements;
	};

	struct RigidBody
	{
		std::wstring name;
		std::wstring nameEng;
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
		std::wstring name;
		std::wstring nameEng;
		unsigned char jointType;
		NormalJoint jointData;
	};

	struct ModelDataDesc
	{
		std::wstring modelFilePath;
		Header header;
		ModelInfo modelInfo;
		std::vector<Vertex> vertices;
		std::vector<Index> indexies;
		std::vector<PMX::Texture> textures;
		std::vector<Material> materials;
		std::vector<BoneData> bones;
		std::vector<Morph> morphs;
		std::vector<DisplayFrame> displayFrame;
		std::vector<RigidBody> rigidBodies;
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