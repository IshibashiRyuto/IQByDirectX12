/*
	Device.h
	D3D12Device���̂��i�[����
	���̂�ComPtr�Ŋi�[����Ă���AComPtr�ł̂����݂̂������Ă���
	(���|�C���^��ComPtr����擾����B���ڎ擾���邱�Ƃ͋����Ȃ�)
	@author Ishibashi Ryuto
	@history
	2018/07/11 ���ō쐬
*/
#pragma once
/* �V�X�e���w�b�_�C���N���[�h */
#include <memory>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;

class Device
{
public:

	/// �f�X�g���N�^
	~Device();

	/// @fn Create
	/// �f�o�C�X���쐬����
	/// @note �f�o�C�X�͂��̃��\�b�h�Ő�������BDevice�̎��̂����̂܂܎����Ƃ͂ł��Ȃ��B
	/// @retval �f�o�C�X�쐬������:Device�̃X�}�[�g�|�C���^, �f�o�C�X�쐬���s��:nullptr
	static std::shared_ptr<Device> Create();

	/// @fn GetDevice
	/// �f�o�C�X��ComPtr���擾����
	/// @retval ComPtr(ID3D12Device) �f�o�C�X��ComPtr
	ComPtr<ID3D12Device> GetDevice() const;

	/// @fn GetFeatureLevel
	/// �f�o�C�X���쐬�����t�B�[�`���[���x�����擾����
	D3D_FEATURE_LEVEL GetFeatureLevel() const;

private:
	/// �R���X�g���N�^
	Device();

	D3D_FEATURE_LEVEL mFeatureLevel;
	ComPtr<ID3D12Device> mDevice;
};

