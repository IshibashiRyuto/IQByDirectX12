/**
	@file	PMDModelData.h
	@brief	PMDModelData�̐錾�Ƃ��̊֘A�\���̏����L�q����
	@author	Ishibashi Ryuto
	@date	2018/08/28	���ō쐬

	@date	2018/12/02	�V���h�E�}�b�v�`���ǉ�
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include<vector>
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include "../../Math/Math.h"
#include "../ModelData.h"

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class ConstantBuffer;
class Device;
class TextureLoader;


/*
*		PMD����`
*		�Q�l: https://blog.goo.ne.jp/torisu_tetosuki
*/

/**
*	@struct PMDHeader
*	@brief	PMD�w�b�_���
*/
struct PMDHeader
{
	float version;			//! �o�[�W�������
	char model_name[20];	//! ���f����
	char comment[256];		//! �R�����g���
};

#pragma pack(1)

/**
*	@struct PMDVertex
*	@brief	PMD���_���
*/
struct PMDVertex
{
	Math::Vector3 position;			//! ���W���
	Math::Vector3 normal;			//! �@�����
	Math::Vector2 uv;				//! uv���
	unsigned short boneIndex[2];	//! �e�����󂯂�{�[���C���f�b�N�X
	unsigned char boneWeight;		//! 1�ڂ̃{�[���ɗ^����e���x(0~100, �{�[��2�̉e���x��100-boneWeight)
	unsigned char edgeFlag;			//! 0:�ʏ�, 1:�G�b�W����
};

/**
*	@struct PMDMaterial
*	@brief	PMD�}�e���A�����
*/
struct PMDMaterial
{
	Math::Vector3 diffuseColor;		//! �����F
	float alpha;					//! �����F�̕s�����x
	float specularity;				//! �X�y�L�����̋���
	Math::Vector3 specularColor;	//! ����F
	Math::Vector3 ambientColor;		//! ���F
	unsigned char toonIndex;		//! �g�p����toon�̔ԍ�
	unsigned char edgeFlag;			//! �֊s�A�e���g�p���邩
	unsigned int faceVertexCount;	//! �ގ����g�p����ʒ��_���X�g�̃f�[�^��
	char textureFileName[20];		//! �e�N�X�`���t�@�C����
};

/**
*	@struct PMDShaderMaterialData
*	@brief	�V�F�[�_�ɓ]�����邽�߂̃}�e���A�����
*/
struct PMDShaderMaterialData
{
	Math::Vector4 diffuseColor;
	float specularity;
	Math::Vector3 specularColor;
	Math::Vector3 ambientColor;
	int isUseTexture;
	int sphereFlag;				// �X�t�B�A�t���O ... 0: ���Z�X�t�B�A, 1: ��Z�X�t�B�A
};

/**
*	@struct	PMDBone
*	@brief	PMD�{�[�����
*/
struct PMDBone
{
	char boneName[20];
	unsigned short parentBoneIndex;
	unsigned short tailBoneIndex;
	unsigned char boneType;
	unsigned short ikParentBoneIndex;
	Math::Vector3 headPos;
};

#pragma pack()

/**
*	@struct	PMDModelInfo
*	@brief	PMD���f�����
*/
struct PMDModelInfo
{
	std::string					modelPath;		//! ���f���̃t�@�C���p�X
	std::vector<PMDVertex>		vertexData;		//!	���_�f�[�^
	std::vector<unsigned short> indexData;		//! �C���f�b�N�X�f�[�^
	std::vector<PMDMaterial>	materials;		//! �}�e���A���f�[�^
	std::vector<PMDBone>		boneData;		//! �{�[���f�[�^
};

class PMDModelData : public ModelData
{
public:
	/**
	*	@brief	�f�X�g���N�^
	*/
	~PMDModelData();
	
	/**
	*	@brief	PMD���f���𐶐�����
	*	@param[in]	device					: dx12�f�o�C�X
	*	@param[in]	modelInfo				: ���f�����
	*	@param[in]	shareToonTextureIndex	: ���L�e�N�X�`���g�D�[���̃C���f�b�N�X
	*	@param[in]	pipelineStateObject		: �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	*	@param[in]	shadowPSO				: �V���h�E�}�b�v�`��pPSO
	*	@param[in]	rootSignature			: ���[�g�V�O�l�`��
	*/
	static std::shared_ptr<PMDModelData> Create(std::shared_ptr<Device> device,
		const PMDModelInfo& modelInfo,
		const std::vector<int> shareToonTextureIndex,
		std::shared_ptr<PipelineStateObject> pipelineStateObject,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature>	rootSignature);

	/**
	*	@brief	���_�����Z�b�g����
	*
	*	@param[in]	vertexData	: �Z�b�g���钸�_���
	*/
	void SetVertexData(const std::vector<PMDVertex>& vertexData);

	/**
	*	@brief	�C���f�b�N�X�����Z�b�g����
	*	
	*	@param[in]	indexData	: �C���f�b�N�X���
	*/
	void SetIndexData(const std::vector<unsigned short>& indexData);

	/**
	*	@brief	�}�e���A���f�[�^���Z�b�g����
	*
	*	@param[in]	device					: dx12�f�o�C�X
	*	@param[in]	materials				: �}�e���A���f�[�^
	*	@param[in]	modelPath				: ���f���̃t�@�C���p�X
	*	@param[in]	shareToonTextureIndex	: ���LToon�e�N�X�`���̃C���f�b�N�X
	*/
	void SetMaterialData(std::shared_ptr<Device> device, const std::vector<PMDMaterial>& materials, const std::string& modelPath, const std::vector<int>& shareToonTextureIndex);

	/**
	*	@brief	�{�[�������Z�b�g����
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	bone		: �{�[���f�[�^
	*/
	void SetBoneData(std::shared_ptr<Device> device, const std::vector<PMDBone>& bone);

	/**
	*	@brief �X�V����
	*/
	void Update();

	/**
	*	@brief		�`�揈��
	*
	*	@param[in]	commandList		: �`��ΏۃR�}���h���X�g
	*	@param[in]	instanceData	: �C���X�^���X���
	*/
	void Draw(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

	/**
	*	@brief		�V���h�E�`�揈��
	*	@param[in]	commandList		: �`��ΏۃR�}���h���X�g
	*	@param[in]	instanceData	: �C���X�^���X���
	*/
	void DrawShadow(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

private:
	static const int MATERIAL_SHADER_RESOURCE_NUM = 5;

	unsigned int					mVertexCount;		//! ���_��
	std::vector<PMDVertex>			mVertex;			//! ���_�f�[�^
	unsigned int					mIndexCount;		//! ���C���f�b�N�X��
	std::vector<unsigned short>		mIndex;				//! �C���f�b�N�X�f�[�^
	unsigned int					mMaterialCount;		//! �}�e���A����
	std::vector<PMDMaterial>		mMaterials;			//! �}�e���A���f�[�^
	std::vector<PMDBone>			mBoneData;			//! �{�[�����
	std::shared_ptr<ConstantBuffer> mMaterialData;		//! �}�e���A���f�[�^�p�萔�o�b�t�@
	std::shared_ptr<TextureLoader>	mTextureLoader;		//! ���f���e�N�X�`�����[�_
	std::shared_ptr<DescriptorHeap> mBoneHeap;			//! �{�[�����p�q�[�v
	std::shared_ptr<ConstantBuffer> mBoneMatrixBuffer;	//! �{�[�����p�o�b�t�@

	/**
	*	@brief	�|�[�Y���X�V����
	*/
	void UpdatePose();

	/**
	*	@brief	�R���X�g���N�^
	*	@param[in]	device					: dx12�f�o�C�X
	*	@param[in]	modelInfo				: ���f�����
	*	@param[in]	shareToonTextureIndex	: ���L�e�N�X�`���g�D�[���̃C���f�b�N�X
	*	@param[in]	pipelineStateObject		: �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	*	@param[in]	shadowPSO				: �V���h�E�}�b�v�`��pPSO
	*	@param[in]	rootSignature			: ���[�g�V�O�l�`��
	*/
	PMDModelData(std::shared_ptr<Device> device,
		const PMDModelInfo& modelInfo,
		const std::vector<int> shareToonTextureIndex,
		std::shared_ptr<PipelineStateObject> pipelineStateObject,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);
};
