#include "ConvertString.h"

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
