#include "ConvertString.h"
#include <vector>
#include <Windows.h>


std::wstring ConvertStringToWString(const std::string & str)
{
	// ワイド文字列のバイトサイズを取得
	auto byteSize = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.data(), -1, nullptr, 0);

	// 変換先の文字列バッファを生成
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
	if (buffer.empty())
	{
		return std::string("WString Convert Failed.");
	}
	WideCharToMultiByte(CP_ACP, 0, str.data(), -1, &buffer.front(), buffer.size(), nullptr, nullptr);
	return std::string(buffer.begin(), buffer.end());
}
