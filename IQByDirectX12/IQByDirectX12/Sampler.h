/*
*	@file Sampler.h
*	@brief ���I�T���v���N���X���L�q�����t�@�C��
*	@author Ishibashi Ryuto
*	@date 
*	2018/09/27	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <d3d12.h>

// �N���X�g�p�錾
class Device;
class DescriptorHeap;


class Sampler
{
public:
	/// @brief �R���X�g���N�^
	Sampler();

	/// @brief �f�X�g���N�^
	~Sampler();

	/// @brief Sampler�C���X�^���X�𐶐�����
	/// @note Sampler�N���X�C���X�^���X�͂��̃��\�b�h�ł̂ݐ����\
	std::shared_ptr<Sampler> Create(const std::shared_ptr<Device> device);
private:
	std::shared_ptr<DescriptorHeap> mSamplerDescriptorHeap;			//! �T���v���[���i�[����f�B�X�N���v�^�q�[�v
};

