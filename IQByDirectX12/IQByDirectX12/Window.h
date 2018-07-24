/*
	Window.h
	ウィンドウズAPIを用いてウィンドウを作成するクラス
	ウィンドウサイズ、ウィンドウハンドル等の管理もこのクラスで行う
	@author Ishibashi Ryuto
	@history
	2018/07/10 初版作成
*/
#pragma once

/*システムヘッダインクルード*/
#include <Windows.h>

class Window
{
public:

	/// コンストラクタ
	/// @param[in] hInstance インスタンスハンドル
	Window(HINSTANCE hInstance);

	/// デストラクタ
	~Window();

	/// @fn GetWindowHandle
	/// ウィンドウハンドルを取得する
	/// @retval HWND ウィンドウハンドル
	/// @author Ishibashi Ryuto
	HWND GetWindowHandle() const;

	/// @fn GetWindowSize
	/// ウィンドウサイズを取得する
	/// @retval ウィンドウサイズの矩形情報
	const RECT& GetWindowRect() const;
private:
	
	/*定数定義*/
	const RECT WINDOW_RECT = { 0, 0, 640, 480 };
	const char* WINDOW_TITLE = "IQByDirectX12";

	/*変数宣言*/
	// ウィンドウハンドル
	HWND mHWnd;
	// インスタンスハンドル
	HINSTANCE mHInst;
	// ウィンドウクラス
	WNDCLASSEX mWndClass;


	/*プライベートメソッド*/

	/// @fn _CreateWindowClassEx
	/// ウィンドウクラスの作成
	/// @note mWndClassに結果が代入される
	/// @retval bool : true 成功, false 失敗
	bool _CreateWindowClassEx();

	/// @fn _CreateWindow
	/// ウィンドウの作成
	/// @note 先に_CreateWindowClassExを使用してウィンドウクラスを作成してください
	/// @retval bool : true 成功, false 失敗
	bool _CreateWindow();
};


/// @fn _WindowProc
/// ウィンドウクラスのプロシージャルメソッド
LRESULT _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);