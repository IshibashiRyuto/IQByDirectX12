/*
*	@file	Sprite.h
*	@brief	Spriteクラスを記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/13	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include<memory>
#include "../Math/Math.h"

///	@class	Sprite
/// @brief	2Dスプライトクラス
///
///	2Dオブジェクトであるスプライトデータを格納するクラス
/// 位置指定、回転指定、倍率指定等設定して描画可能
///	2Dデータであるため、2次元以外の情報を持たないため注意
/// 3D変換として取り扱うことも不可
///	3次元スプライトを取り扱う場合にはPlaneクラスを使用すること
class Sprite
{
public:
	/// @brief	コンストラクタ
	Sprite();

	/// @brief	デストラクタ
	~Sprite();

	/// @brief	スプライトを作成する
	/// @retval	スプライトのスマートポインタ
	static std::shared_ptr<Sprite> Create();
	
	/// @brief	スプライトを描画する
	void Draw() const;

	/// @brief	スプライトの描画位置を設定する
	void SetPosition();

	/// @brief	スプライトの回転角度を設定する
	void SetRotation();

	/// @brief	スプライトの拡大率を設定する

	///	@brief	スプライトに張り付けるテクスチャを変更する
	void SetTexture();

private:

};

