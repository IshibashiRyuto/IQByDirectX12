/*
	@file	TextureLoader.h
	@brief	�e�N�X�`���[���[�_�[
	@author Ishibashi Ryuto
	@date	2018/07/28	���ō쐬
			2018/11/25	�V�o�b�t�@�N���X�ɍ��킹�ċL�q�ύX
*/
#pragma once

/*�V�X�e���w�b�_�C���N���[�h*/
#include <string>
#include <memory>
#include <map>
#include <DirectXTex.h>

/*����w�b�_�C���N���[�h*/
#include "../Dx12/Buffer/Texture.h"
#include "TextureManager.h"

/*�N���X�d�l�錾*/
class CommandQueue;
class GraphicsCommandList;
class Device;

/// @class TextureLoader
/// @brief �e�N�X�`���̓ǂݍ��݂��s���N���X
/// ���̃N���X�̓e�N�X�`���f�[�^��ǂݍ��݁A
/// �e�N�X�`���f�[�^�̊Ǘ����s���B
/// ���łɓǂݍ��ݍς݂̃e�N�X�`����ǂݍ��ލۂ�
/// �ǂݍ��ݍς݃f�[�^��Ԃ��悤�ɂ���
class TextureLoader
{
public:
	/// �f�X�g���N�^
	~TextureLoader();

	/// @fn Create
	/// �e�N�X�`�����[�_�[�𐶐�����
	/// @note TextureLoader�N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	/// ���̂𒼐ڎ����Ƃ͂ł��Ȃ�
	/// @param[in] device�@�F�f�o�C�X
	static std::shared_ptr<TextureLoader> Create(std::shared_ptr<Device> device);

	/// @fn Load
	/// �e�N�X�`�������[�h���A�e�N�X�`���̃n���h����Ԃ�
	/// @param[in] filePath : �e�N�X�`���̃t�@�C���p�X
	/// @retval �ǂݍ��ݐ����� : �e�N�X�`���Ǘ��n���h��
	/// @retval �ǂݍ��ݎ��s�� : -1
	int Load(const std::string& filePath);
	int Load(const std::wstring& filePath);
private:
	/* �ϐ��錾 */
	std::map< std::wstring, int> mTextureHandleManager;			// �ǂݍ��ݍς݃e�N�X�`���̃n���h�����Ǘ�����
	std::shared_ptr<Device> mDevice;							// �f�o�C�X
	TextureManager &mTextureManager;							// �e�N�X�`���}�l�[�W���ւ̎Q��

	ComPtr<ID3D12Resource> mUpdateBuffer;						// UpdateSubResource�Ŏg�p

	/*���[�J�����\�b�h*/

	/// �R���X�g���N�^
	TextureLoader(std::shared_ptr<Device> device);

	/// @fn LoadTGATexture
	/// @brief		TGA�e�N�X�`����ǂݍ���
	/// @param[in]	filePath	: �t�@�C���p�X
	/// @param[out]	metaData	: ���^�f�[�^
	/// @param[out] image		: �C���[�W�f�[�^
	/// @retval		�ǂݍ��ݐ����̉�
	bool LoadTGATexture(std::wstring filePath, DirectX::TexMetadata* metaData, DirectX::ScratchImage& imageData) const;

	/// @fn LoadDDSTexture
	/// @brief		DDS�e�N�X�`����ǂݍ���
	/// @param[in]	filePath	: �t�@�C���p�X
	/// @param[out]	metaData	: ���^�f�[�^
	/// @param[out] image		: �C���[�W�f�[�^
	/// @retval		�ǂݍ��ݐ����̉�
	bool LoadDDSTexture(std::wstring filePath, DirectX::TexMetadata* metaData, DirectX::ScratchImage& imageData) const;

	/// @fn LoadWICTexture
	/// @brief		WIC�e�N�X�`����ǂݍ���
	/// @param[in]	filePath	: �t�@�C���p�X
	/// @param[out]	metaData	: ���^�f�[�^
	/// @param[out] image		: �C���[�W�f�[�^
	/// @retval		�ǂݍ��ݐ����̉�
	bool LoadWICTexture(std::wstring filePath, DirectX::TexMetadata* metaData, DirectX::ScratchImage& imageData) const;
};

