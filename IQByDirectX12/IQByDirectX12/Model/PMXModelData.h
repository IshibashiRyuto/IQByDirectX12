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
#include <wrl.h>
#include <d3d12.h>

// ����w�b�_�C���N���[�h
#include "ModelData.h"
#include "../Math/Math.h"

// �N���X�g�p�錾
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class TextureLoader;
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
	};

	struct Vertex
	{
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector2 uv;
		std::vector<Math::Vector4> appendUV;
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

	struct Bone
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
		std::wstring name;
		std::wstring nameEng;
		unsigned char specialFrameFlag;
		int FrameDataCount;
		std::vector<FrameData> frameData;
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
		std::vector<Bone> bones;
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


class PMXModelData : public ModelData
{
public:
	PMXModelData(ComPtr<ID3D12Device> device,
		std::vector<PMX::Vertex> vertexData,
		std::vector<PMX::Index> indexData,
		int materialCount);
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
	static std::shared_ptr<PMXModelData> Create(ComPtr<ID3D12Device> device, const PMX::ModelDataDesc& modelDataDesc);

	/// @fn Draw
	/// �`�揈��
	/// @param[in]	graphicsCommandList	: �R�}���h���X�g
	/// @param[in]	instanceData		: �C���X�^���X�f�[�^
	void Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;

private:
	/* �萔��` */

	/* �ϐ��錾 */
	std::shared_ptr<ConstantBuffer> mMaterialDataBuffer;			// �}�e���A���f�[�^��ۑ�����萔�o�b�t�@
	std::vector<PMX::MaterialData> mMaterialData;
	std::shared_ptr<TextureLoader> mTextureLoader;
	std::vector<int>				mTextureHandle;		// �e�N�X�`���n���h��

	/* ���[�J�����\�b�h��` */

	/// @fn LoadModelTexture
	/// ���f���̃e�N�X�`���f�[�^�����[�h����
	/// @param[in] textures �e�N�X�`���f�[�^�z��
	/// @param[in] modelFilePath ���f���t�@�C���p�X
	void LoadModelTexture(const std::vector<PMX::Texture> & textures, const std::wstring& modelFilePath);

	/// @fn SetMaterial
	/// �}�e���A���f�[�^���Z�b�g����
	/// @param[in] materials �}�e���A���f�[�^�z��
	void SetMaterial(const std::vector<PMX::Material>& materials);

};