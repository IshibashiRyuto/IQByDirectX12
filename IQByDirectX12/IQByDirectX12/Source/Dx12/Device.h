/*
	@file	Device.h
	@brief	Device�̐錾���L�q����
	@author Ishibashi Ryuto
	@date	2018/07/11 ���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <memory>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;

/**
*	@class	Device
*	@brief	ID3D12Device�̃��b�p�[�N���X
*
*	D3D12Device���̂��i�[����
*	���̂�ComPtr�Ŋi�[����Ă���AComPtr�ł̂����݂̂������Ă���
*	(���|�C���^��ComPtr����擾����B���ڎ擾���邱�Ƃ͋����Ȃ�)
*/
class Device
{
public:

	/**
	*	@brief	�f�X�g���N�^
	*/
	~Device();

	/**
	*	@brief	�f�o�C�X���쐬����
	*
	*	@retval �f�o�C�X�쐬������	: Device�̃X�}�[�g�|�C���^
	*	@retval	�f�o�C�X�쐬���s��	: nullptr
	*
	*	@note	�f�o�C�X�͂��̃��\�b�h�Ő�������BDevice�̎��̂����̂܂܎����Ƃ͂ł��Ȃ��B
	*/
	static std::shared_ptr<Device> Create();

	/**
	*	�f�o�C�X��ComPtr���擾����
	*	@retval ComPtr(ID3D12Device) �f�o�C�X��ComPtr
	*/
	ComPtr<ID3D12Device> GetDevice() const;

	/**
	*	�f�o�C�X���쐬�����t�B�[�`���[���x�����擾����
	*/
	D3D_FEATURE_LEVEL GetFeatureLevel() const;

	/**
	*	@brief �f�o�C�X�փA�N�Z�X����I�y���[�^
	*/
	ID3D12Device * const operator->();

private:
	/**
	*	@brief	�R���X�g���N�^
	*/
	Device();

	D3D_FEATURE_LEVEL mFeatureLevel;	//! �f�o�C�X�t���[�`���[���x��
	ComPtr<ID3D12Device> mDevice;		//!	�f�o�C�X����
};

