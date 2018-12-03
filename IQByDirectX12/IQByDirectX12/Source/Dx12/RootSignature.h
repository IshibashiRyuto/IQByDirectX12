/*
	@file	RootSignature.h
	���[�g�V�O�l�`��
	@author Ishibashi Ryuto
	@date	2018/09/05	���ō쐬
			2018/11/29	�o�C���h����DescriptorHeap��ێ�����݌v�ɕύX
*/
#pragma once
// �w�b�_�C���N���[�h
#include<vector>
#include<memory>
#include<map>
#include<d3d12.h>
#include<wrl.h>

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class DescriptorHeap;
class GraphicsCommandList;

/**
*	@class	RootSignature
*	@brief	���[�g�V�O�l�`�����Ǘ�����
*			�����ɕK�v�ȃp�����[�^���ǂ�ǂ�ǉ����āA�Ō�ɍ\�z����B
*			�܂��A���[�g�p�����[�^�Ƀf�B�X�N���v�^���o�C���h����̂����̃N���X
*/
class RootSignature 
{
public:
	/// �R���X�g���N�^
	RootSignature();

	/// �f�X�g���N�^
	~RootSignature();

	/**
	*	���[�g�V�O�l�`���𐶐�����
	*	@note RootSignature�N���X�̎��̂͂��̃��\�b�h�ł̂ݐ����\
	*	@retval		��������	: RootSignature�̃|�C���^
	*	@retval		�������s	: nullptr
	*/
	static std::shared_ptr<RootSignature> Create();

	/**
	*	@brief	���݂̐ݒ荀�ڂŃ��[�g�V�O�l�`�����\�z����
	*	@note	�Ăяo���x��RootSignature���\�z���邽�߁A�d�߁B
	*			��{�I�ɂ�1�񂾂��Ăяo���΂����悤�ɂ��Ă�������
	*	@param[in] device		: D3D12�f�o�C�X
	*	@retval	�\�z����		: true
	*	@retval �\�z���s		: false
	*/
	bool ConstructRootSignature(ComPtr<ID3D12Device> device);

	/**
	*	@brief	�f�B�X�N���v�^�����W��ǉ�����
	*
	*	@param[in] rootParamIndex	: �ǉ��Ώۃ��[�g�p�����[�^�̃C���f�b�N�X
	*	@param[in] descriptorRange	: �f�B�X�N���v�^�����W
	*
	*	@note	���݂��Ȃ����[�g�p�����[�^���Q�Ƃ����ꍇ�A�ǉ��͍s���Ȃ��̂Œ���
	*/
	void AddDescriptorRange(int rootParamIndex, const D3D12_DESCRIPTOR_RANGE & descriptorRange);

	/**
	*	@brief	�f�B�X�N���v�^�����W��ǉ�����
	*
	*	@param[in] rootParamIndex		: �ǉ��Ώۃ��[�g�p�����[�^�̃C���f�b�N�X
	*	@param[in] rangeType			: �f�B�X�N���v�^�̎��
	*	@param[in] descriptorNum		: �ǂݎ��f�B�X�N���v�^�̐�
	*	@param[in] baseShaderRegister	: �V�F�[�_���W�X�^�̊J�n�ԍ�
	*
	*	@note	���݂��Ȃ����[�g�p�����[�^���Q�Ƃ����ꍇ�A�ǉ��͍s���Ȃ��̂Œ���
	*/
	void AddDescriptorRange(int rootParamIndex,
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister);

	
	/**
	*	���[�g�p�����[�^��ǉ�����
	*
	*	@param[in] shaderVisibility	: �V�F�[�_����
	*	@param[in] parameterType	: �p�����[�^�^�C�v(�f�t�H���g��D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
	*
	*	@note	���̃��\�b�h�͂����܂Ń��[�g�p�����[�^���u�ǉ��v���邾���ŁA
	*			��̓I�Ȑݒ�͍s���܂���B
	*			��̓I�ȃf�B�X�N���v�^�����W��AddDescriptorRange���g���Ēǉ����Ă�������
	*
	*	@return	���[�g�p�����[�^�ԍ�
	*/
	int AddRootParameter(D3D12_SHADER_VISIBILITY shaderVisibility, D3D12_ROOT_PARAMETER_TYPE paramType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);

	/**
	*	@brief	���[�g�V�O�l�`�����擾����
	*
	*	@return	���[�g�V�O�l�`����Com�|�C���^
	*/
	ComPtr<ID3D12RootSignature> GetRootSignature() const;

	/**
	*	@brief	���[�g�p�����[�^�Ƀo�C���h����f�X�N���v�^�q�[�v��ݒ肷��
	*
	*	@param[in]	rootParamIndex		: ���[�g�p�����[�^�̃C���f�b�N�X
	*	@param[in]	descHeap			: �Z�b�g����f�X�N���v�^�q�[�v
	*/
	void SetBindDescriptorHeap(unsigned int rootParamIndex, std::shared_ptr<DescriptorHeap> descHeap);

	/**
	*	@brief	���[�g�p�����[�^���R�}���h���X�g�ɃZ�b�g����
	*
	*	@param[in]	commandList		: ���[�g�p�����[�^���Z�b�g����R�}���h���X�g
	*/
	void SetRootParameter(std::shared_ptr<GraphicsCommandList> commandList) const;

	/**
	*	@brief	���[�g�V�O�l�`�����R�}���h���X�g�ɃZ�b�g����
	*/
	void SetRootSignature(std::shared_ptr<GraphicsCommandList> commandList) const;

private:
	D3D12_STATIC_SAMPLER_DESC mStaticSamplerDesc;							//! �ÓI�T���v��
	std::map<int ,std::vector<D3D12_DESCRIPTOR_RANGE> >	mDescriptorRanges;	//! �f�B�X�N���v�^�����W
	std::vector<D3D12_ROOT_PARAMETER> mRootParameters;						//! ���[�g�p�����[�^
	std::vector<std::shared_ptr<DescriptorHeap>> mDescriptorHeap;			//! ���[�g�p�����[�^�Ƀo�C���h����f�X�N���v�^�q�[�v
	ComPtr<ID3D12RootSignature> mRootSignature;								//! ���[�g�V�O�l�`���Q
};

