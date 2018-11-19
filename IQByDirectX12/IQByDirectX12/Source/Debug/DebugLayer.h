/*
*	@file DebugLayer.h
*	@brief デバッグ関連情報を取りまとめたクラス
*	@author Ishibashi Ryuto
*	@date
*	2018/09/25	初版作成
*/
#pragma once
//システムヘッダインクルード
#include <string>

enum class OutputDestination
{
	Console,
	Window,
	Text,
};

class DebugLayer
{
public:
	/// @brief デストラクタ
	~DebugLayer();

	/// @brief シングルトンインスタンスを取得する
	static DebugLayer& GetInstance()
	{
		static DebugLayer inst;
		return inst;
	}

	/// @brief デバッグメッセージを出力する
	/// @@param[in]	message	: メッセージ
	void PrintDebugMessage(const std::string& message);
	void PrintDebugMessage(const std::wstring& message);

	/// @brief		デバッグメッセージの出力先を設定する
	/// @param[in]	OutputDestination	: 出力先
	void SetOutputDestination(OutputDestination outputDestination);

private:
	// 定数定義
	const char* OUTPUT_FILE_NAME{ "DebugInfo.txt" };	//! デバック情報テキストの出力先ファイル

	// 変数宣言
	OutputDestination mOutputDestination{};	//! デバック情報の出力先(デフォルトでコンソール出力)

	// ローカルメソッド定義
	/// @brief コンストラクタ
	DebugLayer();
	DebugLayer(const DebugLayer&) {}
	void operator=(const DebugLayer&){}
};