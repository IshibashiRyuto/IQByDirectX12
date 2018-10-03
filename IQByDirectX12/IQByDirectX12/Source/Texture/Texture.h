/*
	Texture.h
	�e�N�X�`���f�[�^�������N���X
	@author Ishibashi Ryuto
	@param history
	2018/07/28 ���ō쐬
*/
#pragma once
/*�V�X�e���w�b�_�C���N���[�h*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*����w�b�_�C���N���[�h*/

/*�O���錾*/
using Microsoft::WRL::ComPtr;

class Texture
{
public:
	/// �R���X�g���N�^
	Texture();

	/// �f�X�g���N�^
	~Texture();

	/// @fn Create
	/// �e�N�X�`���f�[�^�𐶐�����
	/// @note Texture�N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	/// ���̂𒼐ڎ����Ƃ͂ł��Ȃ�
	/// @param[in] device		D3D12�f�o�C�X
	/// @param[in] textureData	�e�N�X�`���f�[�^�̍��ރ|�C���^
	/// @retval ��������: Texture��shared_ptr
	/// @retval �������s: nullptr
	static std::shared_ptr<Texture> Create(ComPtr<ID3D12Resource> textureData);

	/// @fn GetTextureData
	/// �e�N�X�`���f�[�^���擾����
	/// @retval	�e�N�X�`���f�[�^
	const ComPtr<ID3D12Resource> GetTextureData() const;

	/// @fn GetShaderResourceView
	/// �V�F�[�_�[���\�[�X�r���[���擾����
	/// @retval �V�F�[�_���\�[�X�r���[
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetShaderResourceView() const;

	/// @fn SetShaderResourceView
	/// �V�F�[�_�[���\�[�X�r���[��ݒ肷��
	void SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srv);

private:
	ComPtr<ID3D12Resource> mTextureData;
	D3D12_SHADER_RESOURCE_VIEW_DESC mShaderResourceView;
};
