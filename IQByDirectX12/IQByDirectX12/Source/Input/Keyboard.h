/*
*	@file Keyboard.h
*	@brief �L�[�{�[�h���͂̊Ǘ��N���X
*	@author	Ishibashi Ryuto
*	@date
*	2018/10/03	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <Windows.h>

// �񋓌^��`
///	@brief ���z�L�[�C���f�b�N�X
enum class VirtualKeyIndex
{
	Key0	= 0x30,
	Key1	= 0x31,
	Key2	= 0x32,
	Key3	= 0x33,
	Key4	= 0x34,
	Key5	= 0x35,
	Key6	= 0x36,
	Key7	= 0x37,
	Key8	= 0x38,
	Key9	= 0x39,
	A		= 0x41,
	B		= 0x42,
	C		= 0x43,
	D		= 0x44,
	E		= 0x45,
	F		= 0x46,
	G		= 0x47,
	H		= 0x48,
	I		= 0x49,
	J		= 0x4A,
	K		= 0x4B,
	L		= 0x4C,
	M		= 0x4D,
	N		= 0x4E,
	O		= 0x4F,
	P		= 0x50,
	Q		= 0x51,
	R		= 0x52,
	S		= 0x53,
	T		= 0x54,
	U		= 0x55,
	V		= 0x56,
	W		= 0x57,
	X		= 0x58,
	Y		= 0x59,
	Z		= 0x5A,
	Enter	= 0x0D,
	Shift	= 0x10,
	Control	= 0x11,
	Alt		= 0x12,
	L_Shift	= 0xA0,
	R_Shift	= 0xA1,
	L_Control = 0xA2,
	R_Control = 0xA3,
	L_Alt	= 0xA4,
	R_Alt	= 0xA5,
	Escape	= 0x1B,
	Space	= 0x20,
	Left	= 0x25,
	Up		= 0x26,
	Right	= 0x27,
	Down	= 0x28,
	Numpad0 = 0x60,
	Numpad1 = 0x61,
	Numpad2 = 0x62,
	Numpad3 = 0x63,
	Numpad4 = 0x64,
	Numpad5 = 0x65,
	Numpad6 = 0x66,
	Numpad7 = 0x67,
	Numpad8 = 0x68,
	Numpad9 = 0x69,
	F1		= 0x70,
	F2		= 0x71,
	F3		= 0x72,
	F4		= 0x73,
	F5		= 0x74,
	F6		= 0x75,
	F7		= 0x76,
	F8		= 0x77,
	F9		= 0x78,
	F10		= 0x79,
	F11		= 0x7A,
	F12		= 0x7B,
};

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	/// @brief	�L�[�����X�V����
	void UpdateKeyState();

	/// @brief	�L�[���z����擾����
	/// @retval �L�[256�̏����������z��
	const unsigned char* GetKeyStateAll();

	/// @brief	�L�[��������Ă��邩
	/// @retval	������Ă���: true
	/// @retval	������Ă��Ȃ�: false
	bool IsKeyDown(VirtualKeyIndex keyIndex);

	/// @brief	�L�[�������ꂽ�u�Ԃ�
	/// @retval	�����ꂽ�u�Ԃł���: true
	/// @retval �����ꂽ�u�ԂłȂ�: false
	bool IsKeyTrigger(VirtualKeyIndex keyIndex);

	/// @brief	�L�[�������ꂽ�u�Ԃ�
	///	@retval	�����ꂽ�u�Ԃł���: true
	/// @retval	�����ꂽ�u�ԂłȂ�: false
	bool IsKeyToggle(VirtualKeyIndex keyIndex);
private:
	const static int KEY_COUNT = 256;					// �L�[�̐�
	const static unsigned char KEY_CHECK_DOWN = 0x80;	// �L�[�_�E���`�F�b�N



	unsigned char mKeyState[KEY_COUNT];			// ���݂̃t���[���ł̃L�[�X�e�[�g
	unsigned char mPreKeyState[KEY_COUNT];		// ���O�̃t���[���ł̃L�[�X�e�[�g
};

