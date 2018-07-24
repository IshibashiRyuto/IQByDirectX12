/*
	Window.h
	�E�B���h�E�YAPI��p���ăE�B���h�E���쐬����N���X
	�E�B���h�E�T�C�Y�A�E�B���h�E�n���h�����̊Ǘ������̃N���X�ōs��
	@author Ishibashi Ryuto
	@history
	2018/07/10 ���ō쐬
*/
#pragma once

/*�V�X�e���w�b�_�C���N���[�h*/
#include <Windows.h>

class Window
{
public:

	/// �R���X�g���N�^
	/// @param[in] hInstance �C���X�^���X�n���h��
	Window(HINSTANCE hInstance);

	/// �f�X�g���N�^
	~Window();

	/// @fn GetWindowHandle
	/// �E�B���h�E�n���h�����擾����
	/// @retval HWND �E�B���h�E�n���h��
	/// @author Ishibashi Ryuto
	HWND GetWindowHandle() const;

	/// @fn GetWindowSize
	/// �E�B���h�E�T�C�Y���擾����
	/// @retval �E�B���h�E�T�C�Y�̋�`���
	const RECT& GetWindowRect() const;
private:
	
	/*�萔��`*/
	const RECT WINDOW_RECT = { 0, 0, 640, 480 };
	const char* WINDOW_TITLE = "IQByDirectX12";

	/*�ϐ��錾*/
	// �E�B���h�E�n���h��
	HWND mHWnd;
	// �C���X�^���X�n���h��
	HINSTANCE mHInst;
	// �E�B���h�E�N���X
	WNDCLASSEX mWndClass;


	/*�v���C�x�[�g���\�b�h*/

	/// @fn _CreateWindowClassEx
	/// �E�B���h�E�N���X�̍쐬
	/// @note mWndClass�Ɍ��ʂ���������
	/// @retval bool : true ����, false ���s
	bool _CreateWindowClassEx();

	/// @fn _CreateWindow
	/// �E�B���h�E�̍쐬
	/// @note ���_CreateWindowClassEx���g�p���ăE�B���h�E�N���X���쐬���Ă�������
	/// @retval bool : true ����, false ���s
	bool _CreateWindow();
};


/// @fn _WindowProc
/// �E�B���h�E�N���X�̃v���V�[�W�������\�b�h
LRESULT _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);