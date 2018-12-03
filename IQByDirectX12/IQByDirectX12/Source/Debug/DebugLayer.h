/*
*	@file	DebugLayer.h
*	@brief	DebugLayerの宣言を記述する
*	@author Ishibashi Ryuto
*	@date	2018/09/25	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include <string>

/**
*	@enum	OutputDestination
*	@brief	デバック出力先
*/
enum class OutputDestination
{
	Console,	//! コンソール
	Window,		//! ポップアップウィンドウ
	Text,		//! テキストデータ
};

/**
*	@class	DebugLayer
*	@brief デバッグ関連情報を取りまとめたクラス
*/
class DebugLayer
{
public:
	/**
	*	@brief デストラクタ
	*/
	~DebugLayer();

	/**
	*	@brief シングルトンインスタンスを取得する
	*/
	static DebugLayer& GetInstance()
	{
		static DebugLayer inst;
		return inst;
	}

	/**
	*	@brief デバッグメッセージを出力する
	*
	*	@param[in]	message	: メッセージ
	*/
	void PrintDebugMessage(const std::string& message);
	void PrintDebugMessage(const std::wstring& message);

	/**
	*	@brief		デバッグメッセージの出力先を設定する
	*
	*	@param[in]	OutputDestination	: 出力先
	*/
	void SetOutputDestination(OutputDestination outputDestination);

private:
	// 定数定義
	const char* OUTPUT_FILE_NAME{ "DebugInfo.txt" };	//! デバック情報テキストの出力先ファイル

	/* 変数宣言 */
	OutputDestination mOutputDestination{};				//! デバック情報の出力先(デフォルトでコンソール出力)

	/* ローカルメソッド定義 */

	/**
	*	@brief	初期生成の禁止
	*/
	DebugLayer();

	/**
	*	@brief	コピーコンストラクタを用いてのコピー禁止
	*/
	DebugLayer(const DebugLayer&) {}

	/**
	*	@brief	コピー演算子を用いてのコピー禁止
	*/
	void operator=(const DebugLayer&){}
};