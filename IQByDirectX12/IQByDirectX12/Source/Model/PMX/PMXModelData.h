/*
	@file		PMXModelData.h
	@brief		PMX���f���̃f�[�^���i�[����
	@date		2018/08/28	���ō쐬
				2018/09/09	���_�f�[�^�̍\����ύX
				2018/11/27	DrawNoMat�ǉ�	
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <memory>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "PMXModelDataInfo.h"
#include "../ModelData.h"
#include "../Math/Math.h"
#include "../Motion/Pose.h"

/* �N���X�g�p�錾 */
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class TextureLoader;
class Device;
using Microsoft::WRL::ComPtr;

/**
*	@class	PMXModelData
*	@brief	PMX���f���f�[�^
*/
class PMXModelData : public ModelData
{
public:
	/**
	*	@brief	�f�X�g���N�^
	*/
	~PMXModelData();

	/**
	*	@brief		PMX�̃��f���f�[�^�𐶐�����
	*	@note		PMXModelData�͂��̃N���X��ʂ��Ă̂ݐ����\
	*	@param[in]	device : ID3D12�f�o�C�X
	*	@param[in]	vertexData: ���_���
	*	@param[in]	vertIndexData: ���_�C���f�b�N�X���
	*	@retval		��������: PMXModelData�̃X�}�[�g�|�C���^
	*	@retval		�������s��: nullptr
	*/
	static std::shared_ptr<PMXModelData> Create(std::shared_ptr<Device> device,
		const PMX::ModelDataDesc& modelDataDesc,
		std::shared_ptr<PipelineStateObject> pipelineStateObject,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);

	/**
	*	@brief	���f�����̍X�V����
	*/
	void Update();

	/**
	*	@brief	�`�揈��
	*	@param[in]	graphicsCommandList	: �R�}���h���X�g
	*	@param[in]	instanceData		: �C���X�^���X�f�[�^
	*/
	void Draw(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

	/**
	*	@brief	�}�e���A�����g��Ȃ��`��
	*
	*	@param[in]	graphicsCommandList	: �R�}���h���X�g
	*	@param[in]	instanceData		: �C���X�^���X�f�[�^
	*/
	void DrawShadow(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

private:
	/* �萔��` */

	/* �ϐ��錾 */
	std::shared_ptr<ConstantBuffer> mMaterialDataBuffer;			//! �}�e���A���f�[�^��ۑ�����萔�o�b�t�@
	std::vector<PMX::MaterialData>	mMaterialData;					//! �}�e���A���f�[�^
	std::vector<int>				mTextureHandle;					//! �e�N�X�`���n���h��
	std::shared_ptr<DescriptorHeap> mBoneHeap;						//! �{�[�������i�[����q�[�v
	std::shared_ptr<ConstantBuffer> mBoneMatrixDataBuffer;			//! �{�[���s�����ۑ�����萔�o�b�t�@

	/* ���[�J�����\�b�h��` */

	/**
	*	@brief		���f���̃e�N�X�`���f�[�^�����[�h����
	*	@param[in]	textures		: �e�N�X�`���f�[�^�z��
	*	@param[in]	modelFilePath	: ���f���t�@�C���p�X
	*/
	void LoadModelTexture(const std::vector<PMX::Texture> & textures, const std::wstring& modelFilePath);

	/**
	*	@brief		�}�e���A���f�[�^���Z�b�g����
	*	@param[in]	materials	: �}�e���A���f�[�^�z��
	*/
	void SetMaterial(const std::vector<PMX::Material>& materials, const std::vector<int>& shareToonTextureIndexies);

	/**
	*	@brief		�{�[�������Z�b�g����
	*
	*	@param[in]	bones	: �{�[�����
	*/
	void SetBone(const std::vector<PMX::BoneData>& bones);

	/**
	*	@brief	�{�[�������X�V����
	*/
	void UpdatePose();


	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	device				: dx12�f�o�C�X
	*	@param[in]	vertexData			: ���_���
	*	@param[in]	indexData			: �C���f�b�N�X�f�[�^
	*	@param[in]	materialCount		: �}�e���A����
	*	@param[in]	boneCount			: �{�[����
	*	@param[in]	pipelineStateObject	: �`��pPSO
	*	@param[in]	shadowPSO			: �V���h�E�`��pPSO
	*	@param[in]	rootSignature		: ���[�g�V�O�l�`��
	*/
	PMXModelData(std::shared_ptr<Device> device,
		std::vector<PMX::Vertex> vertexData,
		std::vector<PMX::Index> indexData,
		int materialCount,
		int boneCount,
		std::shared_ptr<PipelineStateObject> pipelineStateObject,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);
};