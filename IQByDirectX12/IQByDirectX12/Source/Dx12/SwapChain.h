/*
*	@file SwapChain.h
*	@brief スワップチェインを管理するクラス
*	@author Ishibashi Ryuto
*	@date
*	2018/09/28	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <dxgi1_4.h>
#include <wrl.h>

// 自作ヘッダインクルード
#include "Window.h"

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class CommandQueue;

class SwapChain
{
public:
	/// @brief コンストラクタ
	SwapChain();

	/// @brief デストラクタ
	~SwapChain();

	/// @brief	インスタンスを生成する
	/// @note	このクラスのインスタンスはこのメソッドを通じてのみ生成可能
	static std::shared_ptr<SwapChain> Create(std::shared_ptr<CommandQueue> commandQueue,const Window& window, unsigned int backBufferNum = 2);

	/// @brief	画面をスワップする
	void Swap();

	/// @brief	現在の画面インデックスを取得する
	unsigned int GetBackBufferIndex();

	/// @brief	スワップチェインを取得する
	ComPtr<IDXGISwapChain1> GetSwapChain() const;
private:
	// 変数宣言
	ComPtr<IDXGIFactory4>	mFactory;		//! DXGIファクトリ
	ComPtr<IDXGISwapChain3>	mSwapChain;		//! スワップチェイン

	// ローカルメソッド定義
	/// @brief ファクトリの生成
	/// @retval true	: 生成成功
	/// @retval false	: 生成失敗
	bool CreateFactory();

	/// @brief スワップチェインの生成
	/// @param[in]	commandQueue	: コマンドキュー
	/// @param[in]	window			: ウィンドウ情報クラス
	/// @param[in]	backBufferNum	: バックバッファ数
	/// @retval true	: 生成成功
	/// @retval false	: 生成失敗
	bool CreateSwapChain(std::shared_ptr<CommandQueue> commandQueue, const Window& window, unsigned int backBufferNum = 2);
};

