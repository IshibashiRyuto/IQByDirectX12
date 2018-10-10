#include "ConvertString.h"
#include <vector>
#include <Windows.h>


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

std::string ConvertWStringToString(const std::wstring & str)
{
	std::vector<char> buffer(WideCharToMultiByte(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.data(), -1, nullptr, 0, nullptr, 0));
	WideCharToMultiByte(CP_ACP, 0, str.data(), -1, &buffer.front(), buffer.size(), nullptr, nullptr);
	return std::string(buffer.begin(), buffer.end());
}
