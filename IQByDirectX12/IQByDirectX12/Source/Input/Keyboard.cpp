#include <windows.h>
#include <algorithm>
#include "Keyboard.h"


Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

void Keyboard::UpdateKeyState()
{
	std::copy(std::begin(mKeyState), std::end(mKeyState), mPreKeyState);
	GetKeyboardState(mKeyState);
}

const unsigned char* Keyboard::GetKeyStateAll()
{
	return mKeyState;
}

bool Keyboard::IsKeyDown(VirtualKeyIndex keyIndex)
{
	return static_cast<bool>(mKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN);
}

bool Keyboard::IsKeyTrigger(VirtualKeyIndex keyIndex)
{
	return static_cast<bool>( ( ~(mKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN) ) & (mPreKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN));
}

bool Keyboard::IsKeyToggle(VirtualKeyIndex keyIndex)
{
	return static_cast<bool>( (mKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN) ^ (mPreKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN));
}
