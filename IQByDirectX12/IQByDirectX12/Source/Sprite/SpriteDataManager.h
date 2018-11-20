/**
*	@file	SpriteDataManager.h
*	@brief	�X�v���C�g�f�[�^�}�l�[�W�����L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/20	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <memory>
#include <map>

/* �N���X�g�p�錾 */
class SpriteData;
class GraphicsCommandList;
class InstancingDataManager;

/**
*	@class	SpriteDataManager
*	@biref	�X�v���C�g�f�[�^���Ǘ�����V���O���g���N���X
*/
class SpriteDataManager
{
public:
	~SpriteDataManager();

	/**
	*	@brief	�V���O���g���C���X�^���X�̎擾
	*	@retval	�X�v���C�g�f�[�^�}�l�[�W���̃C���X�^���X
	*/
	static SpriteDataManager& GetInstance()
	{
		static SpriteDataManager inst;
		return inst;
	}

	/**
	*	@brief	�X�v���C�g�f�[�^��o�^���A�Ǘ��n���h����Ԃ�
	*	@param[in]	spriteData	: �X�v���C�g�f�[�^
	*	@retval		�X�v���C�g�f�[�^�̃n���h��
	*/
	int Regist(std::shared_ptr<SpriteData> spriteData);

	/**
	*	@brief	�w�肵���n���h���̃X�v���C�g�f�[�^���폜����
	*	@param[in]	handle	: �X�v���C�g�n���h��
	*/
	void Erase(int handle);

	/**
	*	@brief	�w�肵���n���h���̃X�v���C�g�f�[�^�����݂��邩�`�F�b�N����
	*	@param[in]	handle	: �n���h��
	*	@retval	���݂���	: true
	*	@retval	���݂��Ȃ�	: false
	*/
	bool IsExist(int handle) const;

	/**
	*	@brief	�Ǘ����Ă���X�v���C�g�f�[�^��`�悷��
	*	@param[in]	commandList	: �R�}���h���X�g
	*/
	void Draw(std::shared_ptr<GraphicsCommandList> commandList);

private:
	/* �萔��` */
	const int DATA_SIGNATURE_SHIFT_NUM = 24;
	const int SPRITE_DATA_SIGNATURE = 0x12 << DATA_SIGNATURE_SHIFT_NUM;

	/* �ϐ��錾 */
	std::map<int, std::shared_ptr<SpriteData>> mData;		//! �Ǘ��f�[�^
	int mNextHandle;										//! ���Ɏg�p����X�v���C�g�f�[�^�n���h��
	InstancingDataManager& mInstancingDataManager;			//! �C���X�^���V���O�f�[�^�Ǘ��N���X�ւ̎Q��

	/* ���[�J�����\�b�h��` */
	/**
	*	@brief	���Ɏg�p����n���h����T�����A�X�V����
	*/
	void UpdateNextHandle();

	/**
	*	@brief	�R���X�g���N�^
	*/
	SpriteDataManager();

	/**
	*	@brief	�R�s�[�R���X�g���N�^
	*/
	SpriteDataManager(const SpriteDataManager&);

	/**
	*	@brief	=���Z�q�ł̃R�s�[�֎~
	*/
	void operator=(const SpriteDataManager&) {}
};

