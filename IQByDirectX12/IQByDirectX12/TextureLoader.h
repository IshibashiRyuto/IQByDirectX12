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

/// @class TextureLoader
/// @brief �e�N�X�`���̓ǂݍ��݂��s���N���X
/// ���̃N���X�̓e�N�X�`���f�[�^��ǂݍ��݁A
/// �e�N�X�`���f�[�^�̊Ǘ����s���B
/// ���łɓǂݍ��ݍς݂̃e�N�X�`����ǂݍ��ލۂ�
/// �ǂݍ��ݍς݃f�[�^��Ԃ��悤�ɂ���
class TextureLoader
{
public:
	/// �R���X�g���N�^
	TextureLoader();

	/// �f�X�g���N�^
	~TextureLoader();

	/// @fn Create
	/// �e�N�X�`�����[�_�[�𐶐�����
	/// @note TextureLoader�N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	/// ���̂𒼐ڎ����Ƃ͂ł��Ȃ�
	static std::shared_ptr<TextureLoader> Create(ComPtr<ID3D12Device> device);

	std::shared_ptr<Texture> Load(const std::string& str);
private:
	/*�ϐ��錾*/
	std::map< std::string, std::shared_ptr<Texture>> mTextureResource;
	ComPtr<ID3D12Device> mDevice;

	/*���[�J�����\�b�h*/

	/// @fn GetWString
	/// ������������O������ɕϊ�����
	/// @param[in]	1�o�C�g������
	/// @retval		wstring
	std::wstring GetWString(const std::string& str);

	/// @fn UpdateTextureSubresource
	/// �e�N�X�`���̃T�u���\�[�X���A�b�v�f�[�g����
	/// @param[in]	resource ID3D12Resource�̃|�C���^
	/// @param[in]	subresource �T�u���\�[�X�f�[�^
	void UpdateTextureSubresource(ComPtr<ID3D12Resource> resource, D3D12_SUBRESOURCE_DATA& subresource);
};

