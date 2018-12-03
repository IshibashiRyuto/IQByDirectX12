/**
*	@file	PMXLoader.h
*	@brief	PMXLoader�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/08/29	���ō쐬
*/
#pragma once

/* �w�b�_�C���N���[�h */
#include <cstdio>
#include <string>
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include "../ModelLoader.h"
#include "PMXModelData.h"
#include "../Math/Math.h"

/* �N���X�g�p�錾 */
using Microsoft::WRL::ComPtr;
class Device;

/**
*	@class	PMXLoader
*	@brief	PMX�f�[�^�̓Ǎ����Ǘ�����
*/
class PMXLoader : public ModelLoader
{
public:
	/**
	*	@brief	�f�X�g���N�^
	*/
	~PMXLoader();

	/**
	*	@brief		PMX���[�_���쐬����
	*
	*	@param[in]	device				: dx12�f�o�C�X�N���X
	*	@param[in]	shareToonFolderPath	: ���LToon�̃t�H���_�p�X
	*
	*	@note		PMXLoader�N���X�͂��̃��\�b�h��ʂ��Ă̂ݎ��̉��ł���
	*/
	static std::shared_ptr<PMXLoader> Create(std::shared_ptr<Device> device, const std::string& shareToonFolderPath);

	/**
	*	@brief	PMX���f�������[�h����
	*
	*	@param[in]	filePath		: �t�@�C���p�X
	*	@param[in]	pso				: ���f���`��p�p�C�v���C��
	*	@param[in]	shadowPSO		: �V���h�E�`��p�p�C�v���C��
	*	@param[in]	rootSignature	: ���[�g�V�O�l�`��
	*/
	std::shared_ptr<Model> LoadModel(const std::string& filePath, std::shared_ptr<PipelineStateObject> pso,
		std::shared_ptr<PipelineStateObject> shadowPSO, std::shared_ptr<RootSignature> rootSignature);

	/**
	*	@brief	���f���f�[�^���폜����
	*/
	void ClearModelData();

private:
	/* �萔��` */
	const std::string FILE_SIGNATURE = "PMX ";			//!	�t�@�C���V�O�l�`��
	const size_t SIGNATURE_SIZE = sizeof(char) * 4;		//!	�t�@�C���V�O�l�`���̃T�C�Y
	const int SHARE_TOON_NUM = 11;						//!	���L�g�D�[���̐�
	const std::string SHARE_TOON_PATH [11]
	{ 
		"toon0.bmp",
		"toon01.bmp",
		"toon02.bmp",
		"toon03.bmp",
		"toon04.bmp",
		"toon05.bmp",
		"toon06.bmp",
		"toon07.bmp",
		"toon08.bmp",
		"toon09.bmp",
		"toon10.bmp",
	};			//! ���Ltoon�̃t�@�C����

	/* �ϐ��錾 */
	std::vector<int> mShareToonTextureHandle;		// ���L�g�D�[���e�N�X�`���̃n���h��


	/* ���[�J�����\�b�h��` */
	/**
	*	@brief	������o�b�t�@�f�[�^���擾����
	*	@param[in]	fp: �t�@�C���|�C���^
	*	@note	�e�L�X�g�f�[�^��4 + n[Byte]�Ŋi�[����Ă�����̂Ƃ���B
	*			�ŏ���4[Byte]�Ƀo�b�t�@�T�C�Y���A����n[Byte]�ɕ�����f�[�^���i�[����Ă���
	*/
	std::string ReadTextBuf(FILE *fp);

	/**
	*	@brief	���C�h������o�b�t�@�f�[�^���擾����
	*	@param[in]	fp: �t�@�C���|�C���^
	*/
	std::wstring ReadTextBufWString(FILE *fp);

	/**
	*	@biref	�w�b�_�f�[�^��ǂݍ���
	*	@param[in] header : PMX�̃w�b�_
	*	@param[in] fp: �t�@�C���|�C���^
	*/
	void LoadHeader(PMX::Header& header, FILE* fp);

	/**
	*	@brief	���f������ǂݍ���
	*	@param[in] modelInfo	: ���f�����\����
	*	@param[in] fp			: �t�@�C���|�C���^
	*/
	void LoadModelInfo(PMX::ModelInfo& modelInfo, FILE* fp);

	/**
	*	@brief	���_�f�[�^��ǂݍ���
	*	@param[in] vertexData	: ���_�f�[�^�̃x�N�^
	*	@param[in] header		: PMX�t�@�C���w�b�_
	*	@param[in] fp			: �t�@�C���|�C���^
	*/
	void LoadVertexData(std::vector<PMX::Vertex>& vertexData, const PMX::Header& header, FILE* fp);

	/**
	*	@brief	���_�C���f�b�N�X�f�[�^��ǂݍ���
	*	@param[in] indexData	: ���_�C���f�b�N�X�f�[�^�̃x�N�^
	*	@param[in] header		: PMX�t�@�C���w�b�_
	*	@param[in] fp			: �t�@�C���|�C���^
	*/
	void LoadIndexData(std::vector<PMX::Index>& indexData, const PMX::Header& header, FILE* fp);

	/**
	*	@brief	�e�N�X�`������ǂݍ���
	*	@param[in] textureData	: �e�N�X�`���f�[�^�̃x�N�^
	*	@param[in] modelPath	: ���f���̃t�@�C���p�X
	*	@param[in] fp			: �t�@�C���|�C���^
	*/
	void LoadTextureData(std::vector<PMX::Texture>& textureData, const std::wstring& modelPath, FILE* fp);

	/**
	*	@brief	�}�e���A������ǂݍ���
	*	@param[in] materialData		: �}�e���A���f�[�^�̃x�N�^
	*	@param[in] header			: PMX�t�@�C���w�b�_
	*	@param[in] fp				: �t�@�C���|�C���^
	*/
	void LoadMaterial(std::vector<PMX::Material>& materialData, const PMX::Header& header, FILE* fp);

	/**
	*	@brief	�{�[������ǂݍ���
	*	@param[in]	boneData		: �{�[���f�[�^�z��
	*	@param[in]	boneIndexSize	: �{�[���C���f�b�N�X�̃T�C�Y
	*	@param[in]	fp				: �t�@�C���|�C���^
	*/
	void LoadBone(std::vector<PMX::BoneData> & boneData, size_t boneIndexSize, FILE *fp);

	/**
	*	@brief	���[�t����ǂݍ���
	*	@param[in] morphData	: ���[�t�f�[�^�z��
	*	@param[in] header		: PMX�t�@�C���w�b�_
	*/
	void LoadMorph(std::vector<PMX::Morph> & morphData, const PMX::Header& header, FILE* fp);

	/**
	*	@brief	�\���g��ǂݍ���
	*	@param[in] displayFrameData	:�\���g�f�[�^�z��
	*	@param[in] boneIndexSize	: �{�[���C���f�b�N�X�̃T�C�Y
	*	@param[in] morphIndexSize	: ���[�t�C���f�b�N�X�̃T�C�Y
	*	@param[in] fp				: �t�@�C���|�C���^
	*/
	void LoadDisplayFrame(std::vector<PMX::DisplayFrame> & displayFrameData, size_t boneIndexSize, size_t morphIndexSize, FILE *fp);

	/**
	*	@brief		���L�g�D�[���e�N�X�`����ǂݍ���
	*	@param[in]	folderPath	: ���L�g�D�[���e�N�X�`���̂���t�H���_�p�X
	*/
	void LoadShareToon(const std::string& folderPath);

	/**
	*	@brief	�R���X�g���N�^
	*/
	PMXLoader(std::shared_ptr<Device> device);
};

