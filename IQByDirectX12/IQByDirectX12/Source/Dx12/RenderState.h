/*
*	@file RenderState.h
*	@brief �����_�[�X�e�[�g�����܂Ƃ߂��t�@�C��
*	@author Ishibashi Ryuto
*	@date
*	2018/09/26 ���ō쐬
*/
#pragma once

/* �w�b�_�C���N���[�h */
#include <d3d12.h>
#include <map>

const int ALPHA_TYPE_COUNT = 5;
/// @brief �A���t�@�u�����h�I�v�V����
enum class AlphaBlendType
{
	Opacity = 0,	//! �s����
	Blend = 1,		//! ����
	Add = 2,		//! ���Z
	Sub = 3,		//! ���Z
	Mul = 4,		//! ��Z
};

/// @brief �T���v�����O����ۂ̃T���v�����O���@
enum class TextureFilterType 
{
	Nearest = 0,	//! �j�A���X�g���
	Linear = 1,		//! �o�C���j�A���
};

/// @brief UV�l��0�ȉ���������1�ȏ�̏ꍇ�̈�����
enum class TextureWrapType
{
	Repeat = 0,		//! �J��Ԃ�
	Clamp = 1,		//! �ł��؂�
};

const int CULLING_TYPE_COUNT = 3;
enum class CullingType
{
	Front = 0,
	Back = 1,
	Double = 2,
};

///	@class	RenderState
///	@brief	�����_�[�X�e�[�g���W�񂵂��N���X
///
///	�����_�����O�����W�񂵂��N���X
/// �ݒ肵���������ƂɁAPipelineStateDesc���\�z���ĕԂ����\�b�h������
class RenderState
{
public:
	bool				depthTest	: 1;	//! �[�x�e�X�g���s����	
	bool				depthWrite	: 1;	//! �[�x�o�b�t�@�ւ̏������݂��s����
	AlphaBlendType		alphaBlendType;			//! �A���t�@�u�����h
	CullingType			cullingType;		//! �J�����O�^�C�v
	TextureFilterType	textureFilterType;	//! �T���v�����O����ۂ̃T���v�����O���@
	TextureWrapType		textureWrapType;	//! UV�l��0�ȉ���������1�ȏ�̏ꍇ�̈�����
	D3D12_COMPARISON_FUNC depthFunc;		//! �f�v�X�̔�r�֐�
	/// @brief	���ݐݒ肳�ꂽ��񂩂�p�C�v���C���X�e�[�g�f�X�N���\�z���Ď擾����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GetPipelineStateDesc() const;
	
private:
	static bool mIsInitialize;				//! �������ς݂�
	static std::map<CullingType, D3D12_RASTERIZER_DESC>	mRasterizerStates;					//! ���X�^���C�U�X�e�[�g
	static std::map<bool, std::map<bool, D3D12_DEPTH_STENCIL_DESC>>	mDepthStencilStates;	//! �f�v�X�X�e���V���X�e�[�g
	static std::map<AlphaBlendType, D3D12_BLEND_DESC>	mBlendStates;						//! �u�����h�X�e�[�g
	
	/// @brief	�X�e�[�g���e�[�u���̏�����
	void InitStateTable() const;
};
