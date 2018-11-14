/*
*	@file SwapChain.h
*	@brief �X���b�v�`�F�C�����Ǘ�����N���X
*	@author Ishibashi Ryuto
*	@date
*	2018/09/28	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <dxgi1_4.h>
#include <wrl.h>

// ����w�b�_�C���N���[�h
#include "Window.h"

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class CommandQueue;

class SwapChain
{
public:
	/// @brief �R���X�g���N�^
	SwapChain();

	/// @brief �f�X�g���N�^
	~SwapChain();

	/// @brief	�C���X�^���X�𐶐�����
	/// @note	���̃N���X�̃C���X�^���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	static std::shared_ptr<SwapChain> Create(std::shared_ptr<CommandQueue> commandQueue,const Window& window, unsigned int backBufferNum = 2);

	/// @brief	��ʂ��X���b�v����
	void Swap();

	/// @brief	���݂̉�ʃC���f�b�N�X���擾����
	unsigned int GetBackBufferIndex();

	/// @brief	�X���b�v�`�F�C�����擾����
	ComPtr<IDXGISwapChain1> GetSwapChain() const;
private:
	// �ϐ��錾
	ComPtr<IDXGIFactory4>	mFactory;		//! DXGI�t�@�N�g��
	ComPtr<IDXGISwapChain3>	mSwapChain;		//! �X���b�v�`�F�C��

	// ���[�J�����\�b�h��`
	/// @brief �t�@�N�g���̐���
	/// @retval true	: ��������
	/// @retval false	: �������s
	bool CreateFactory();

	/// @brief �X���b�v�`�F�C���̐���
	/// @param[in]	commandQueue	: �R�}���h�L���[
	/// @param[in]	window			: �E�B���h�E���N���X
	/// @param[in]	backBufferNum	: �o�b�N�o�b�t�@��
	/// @retval true	: ��������
	/// @retval false	: �������s
	bool CreateSwapChain(std::shared_ptr<CommandQueue> commandQueue, const Window& window, unsigned int backBufferNum = 2);
};

