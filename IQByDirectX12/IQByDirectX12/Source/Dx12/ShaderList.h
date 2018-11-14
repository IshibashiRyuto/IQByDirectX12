/*
*	@file	ShaderList.h
*	@brief	PSO�ɃZ�b�g����V�F�[�_���X�g�\���̂��L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/14	���ō쐬
*/
#pragma once

#include "../Shader.h"


/// @struct	ShaderList
/// @brief	PSO�ɓ�����V�F�[�_���X�g
struct ShaderList
{
	std::shared_ptr<Shader> VS;		//! �o�[�e�b�N�X�V�F�[�_
	std::shared_ptr<Shader> PS;		//! �s�N�Z���V�F�[�_
	std::shared_ptr<Shader> GS;		//! �W�I���g���V�F�[�_
	std::shared_ptr<Shader> DS;		//! �h���C���V�F�[�_
	std::shared_ptr<Shader> HS;		//! �n���V�F�[�_
};
