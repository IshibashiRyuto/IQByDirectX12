/*
	Shader.h
	�V�F�[�_�����Ǘ�����N���X
	@author Ishibashi Ryuto
	@date
	2018/09/06	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <string>
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// ����w�b�_�C���N���[�h

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;

class Shader
{
public:
	/// �f�X�g���N�^
	~Shader();

	/// @fn Create
	/// �V�F�[�_�N���X�𐶐�����
	/// @note Shader�N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	/// @param[in] filePath		:	�V�F�[�_�t�@�C���p�X
	/// @param[in] entryPoint	:	�V�F�[�_�G���g���|�C���g
	/// @param[in] target		:	�^�[�Q�b�g
	///	@retval		��������	:	Shader�̃|�C���^
	/// @retval		�������s	:	nullptr
	static std::shared_ptr<Shader> Create(const std::wstring& filePath, const std::string& entryPoint, const std::string& target);

	/// @fn GetShaderByteCode
	/// �V�F�[�_�R�[�h���擾����
	/// @retval �V�F�[�_�o�C�g�R�[�h
	const D3D12_SHADER_BYTECODE & GetShaderByteCode() const;

	/// @fn LoadShader
	/// �V�F�[�_�����[�h����
	void LoadShader(const std::wstring & filePath, const std::string& entryPoint, const std::string& compileTarget);

private:
	/* �����o�ϐ���` */
	ComPtr<ID3DBlob> mShaderCode;
	D3D12_SHADER_BYTECODE mShaderByteCode;

	/* ���[�J�����\�b�h��` */
	/// �R���X�g���N�^
	Shader();

};

