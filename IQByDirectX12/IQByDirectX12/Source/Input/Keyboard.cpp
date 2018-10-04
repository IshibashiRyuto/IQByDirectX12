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

const unsigned char* Keyboard::GetKeyStateAll() const
{
	return mKeyState;
}

bool Keyboard::IsKeyDown(VirtualKeyIndex keyIndex) const
{
	return static_cast<bool>(mKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN);
}

bool Keyboard::IsKeyTrigger(VirtualKeyIndex keyIndex) const
{
	return static_cast<bool>( ( ~(mKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN) ) & (mPreKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN));
}

bool Keyboard::IsKeyToggle(VirtualKeyIndex keyIndex) const
{
	return static_cast<bool>( (mKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN) ^ (mPreKeyState[static_cast<int>(keyIndex)] & KEY_CHECK_DOWN));
}

std::shared_ptr<Keyboard> Keyboard::Create()
{
	return std::shared_ptr<Keyboard>(new Keyboard());
}
