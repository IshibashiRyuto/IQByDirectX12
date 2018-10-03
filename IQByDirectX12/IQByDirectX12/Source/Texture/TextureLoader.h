/*
	TextureLoader.h
	�e�N�X�`���[���[�_�[
	@author Ishibashi Ryuto
	@param history
	2018/07/28 ���ō쐬
*/
#pragma once

/*�V�X�e���w�b�_�C���N���[�h*/
#include <string>
#include <memory>
#include <map>

/*����w�b�_�C���N���[�h*/
#include "Texture.h"
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
	/// @retval int :		�e�N�X�`���Ǘ��n���h��
	int Load(const std::string& filePath);
	int Load(const std::wstring& filePath);
private:
	/*�ϐ��錾*/
	std::map< std::wstring, int> mTextureHandleManager;			// �ǂݍ��ݍς݃e�N�X�`���̃n���h�����Ǘ�����
	std::shared_ptr<Device> mDevice;								// �f�o�C�X
	TextureManager &mTextureManager;							// �e�N�X�`���}�l�[�W���ւ̎Q��
	std::shared_ptr<GraphicsCommandList> mCommandList;			// �R�}���h���X�g
	std::shared_ptr<CommandQueue>	mCommandQueue;				// �R�}���h�L���[

	ComPtr<ID3D12Resource> mUpdateBuffer;						// UpdateSubResource�Ŏg�p

	/*���[�J�����\�b�h*/

	/// �R���X�g���N�^
	TextureLoader(std::shared_ptr<Device> device);

	/// @fn UpdateTextureSubresource
	/// �e�N�X�`���̃T�u���\�[�X���A�b�v�f�[�g����
	/// @param[in]	resource ID3D12Resource�̃|�C���^
	/// @param[in]	subresource �T�u���\�[�X�f�[�^
	void UpdateTextureSubresource(ComPtr<ID3D12Resource> resource, D3D12_SUBRESOURCE_DATA& subresource);
};

