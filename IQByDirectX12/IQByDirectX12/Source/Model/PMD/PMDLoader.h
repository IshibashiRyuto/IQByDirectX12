/**
*	@file	PMDLoader.h
*	@brief	PMDLoader�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/08/28	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <memory>
#include "../ModelLoader.h"
#include "PMDModelData.h"

/**
*	@class	PMDLoader
*	@brief	PMD���f���̓Ǎ����Ǘ�����
*
*	PMD���f���Ǎ����Ǘ�����N���X
*	�ǂݍ��ݍς݃f�[�^�̃C���f�b�N�X�����̃N���X���ێ����A
*	���̃N���X�̃f�X�g���N�^�œo�^����������
*	�f�[�^���������
*/
class PMDLoader : public ModelLoader
{
public:
	/**
	*	@brief	�f�X�g���N�^
	*/
	~PMDLoader();

	/**
	*	@brief	PMDLoader���쐬����
	*
	*	@param[in]	device				: dx12�f�o�C�X
	*	@param[in]	shareToonFolderPath	: ���LToon�̃t�H���_�p�X
	*
	*	@retval		��������	: PMDLoader�̃X�}�[�g�|�C���^
	*	@retval		�������s	: nullptr
	*/
	static std::shared_ptr<PMDLoader> Create(std::shared_ptr<Device> device, const std::string& shareToonFolderPath);

	/**
	*	@brief	���f�������[�h����
	*
	*	@param[in]	filePath		: �t�@�C���p�X
	*	@param[in]	pso				: ���f���`��pPSO
	*	@param[in]	shadowPSO		: �V���h�E�}�b�v�`��pPSO
	*	@param[in]	rootSignature	: ���[�g�V�O�l�`��
	*/
	std::shared_ptr<Model> LoadModel(const std::string& filePath, std::shared_ptr<PipelineStateObject> pso,
		std::shared_ptr<PipelineStateObject> shadowPSO, std::shared_ptr<RootSignature> rootSignature);

	/**
	*	@brief	�ǂݍ��񂾃��f�������폜����
	*/
	void ClearModelData();

private:
	/**
	*	@brief	�R���X�g���N�^
	*/
	PMDLoader(std::shared_ptr<Device> device);

	/**
	*	@brief	�R�s�[�R���X�g���N�^�ł̃R�s�[�֎~
	*/
	PMDLoader(const PMDLoader&);

	/**
	*	@brief	�R�s�[���Z�q�ł̃R�s�[�֎~
	*/
	void operator=(const PMDLoader&) {}

	const int SHARE_TOON_NUM = 11;			//! ���LToon�̐�
	const std::string SHARE_TOON_PATH[11]	//! ���LToon�̉摜��
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
	};		

	std::vector<int> mShareToonTextureHandle;	//! ���LToon�e�X�N�`���n���h��
	
	/**
	*	@brief	���LToon�����[�h����
	*	
	*	@param[in]	���LToon�̂���t�H���_�p�X
	*/
	void LoadShareToon(const std::string& toonFolderPath);

	/**
	*	@brief	Toon�f�[�^�����[�h����
	*
	*	@param[in]	fp					: �t�@�C���|�C���^
	*	@param[in]	toonTextureHandle	: 
	*/
	void LoadToonData(FILE *fp, const std::string& modelPath, std::vector<int>& toonTextureHandle);
};

