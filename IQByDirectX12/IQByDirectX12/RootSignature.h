/*
	RootSignature.h
	���[�g�V�O�l�`��
	@author Ishibashi Ryuto
	@date
	2018/09/05	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include<vector>
#include<memory>
#include<map>
#include<d3d12.h>
#include<wrl.h>

// ����w�b�_�C���N���[�h

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;

class RootSignature 
{
public:
	/// �R���X�g���N�^
	RootSignature();

	/// �f�X�g���N�^
	~RootSignature();

	/// @fn Create
	/// ���[�g�V�O�l�`���𐶐�����
	/// @note RootSignature�N���X�̎��̂͂��̃��\�b�h�ł̂ݐ����\
	/// @retval		��������	: RootSignature�̃|�C���^
	/// @retval		�������s	: nullptr
	static std::shared_ptr<RootSignature> Create();

	/// @fn ConstructRootSignature
	/// ���݂̐ݒ荀�ڂŃ��[�g�V�O�l�`�����\�z����
	/// @note �Ăяo���x��RootSignature���\�z���邽�߁A�d�߁B
	///		��{�I�ɂ�1�񂾂��Ăяo���΂����悤�ɂ��Ă�������
	/// @param[in] device		: D3D12�f�o�C�X
	/// @retval	�\�z����		: true
	/// @retval �\�z���s		: false
	bool ConstructRootSignature(ComPtr<ID3D12Device> device);

	/// @fn AddDescriptorRange
	/// �f�B�X�N���v�^�����W��ǉ�����
	/// @note ���݂��Ȃ����[�g�p�����[�^���Q�Ƃ����ꍇ�A�ǉ��͍s���Ȃ��̂Œ���
	/// @param[in] rootParamIndex	: �ǉ��Ώۃ��[�g�p�����[�^�̃C���f�b�N�X
	/// @param[in] descriptorRange	: �f�B�X�N���v�^�����W
	void AddDescriptorRange(int rootParamIndex, const D3D12_DESCRIPTOR_RANGE & descriptorRange);

	/// @param[in] rootParamIndex		: �ǉ��Ώۃ��[�g�p�����[�^�̃C���f�b�N�X
	/// @param[in] rangeType			: �f�B�X�N���v�^�̎��
	/// @param[in] descriptorNum		: �ǂݎ��o�X�N���v�^�̐�
	/// @param[in] baseShaderRegister	: �V�F�[�_���W�X�^�̊J�n�ԍ�
	void AddDescriptorRange(int rootParamIndex,
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister);

	/// @fn AddRootParameter
	/// ���[�g�p�����[�^��ǉ�����
	/// @param[in] parameterType	: �p�����[�^�^�C�v(�f�t�H���g��D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
	/// @param[in] shaderVisibility	: �V�F�[�_����(
	/// @note ���̃��\�b�h�͂����܂Ń��[�g�p�����[�^���u�ǉ��v���邾���ŁA
	///			��̓I�Ȑݒ�͍s���܂���B
	///			��̓I�ȃf�B�X�N���v�^�����W��AddDescriptorRange���g���Ēǉ����Ă�������
	/// @retval	���[�g�p�����[�^�ԍ�
	int AddRootParameter(D3D12_SHADER_VISIBILITY shaderVisibility, D3D12_ROOT_PARAMETER_TYPE paramType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);

	/// @fn GetRootSignature
	/// ���[�g�V�O�l�`�����擾����
	ComPtr<ID3D12RootSignature> GetRootSignature() const;

private:
	D3D12_STATIC_SAMPLER_DESC mStaticSamplerDesc;			// �ÓI�T���v��
	std::map<int ,std::vector<D3D12_DESCRIPTOR_RANGE> >	mDescriptorRanges;	// �f�B�X�N���v�^�����W
	std::vector<D3D12_ROOT_PARAMETER> mRootParameters;		// ���[�g�p�����[�^
	ComPtr<ID3D12RootSignature> mRootSignature;				// ���[�g�V�O�l�`���Q
};

