#include "ConvertString.h"

std::wstring ConvertStringToWString(const std::string & str)
{
	// ���C�h������̃o�C�g�T�C�Y���擾
	auto byteSize = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.data(), -1, nullptr, 0);

	// �ϊ���̕�����o�b�t�@�𐶐�
	std::wstring wstr;
	wstr.resize(byteSize);

	byteSize = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.data(), -1, &wstr[0], byteSize);

	return wstr;
}
