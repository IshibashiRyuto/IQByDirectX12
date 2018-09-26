/*
*	@file RenderState.h
*	@brief レンダーステート情報をまとめたファイル
*	@author Ishibashi Ryuto
*	@date
*	2018/09/26 初版作成
*/
#pragma once

/// @brief アルファブレンドオプション
enum class AlphaBlendType
{
	Opacity = 0,	//! 不透明
	Blend = 1,		//! 透明
	Add = 2,		//! 加算
	Sub = 3,		//! 減算
	Mul = 4,		//! 乗算
};

/// @brief テクスチャのフィルタタイプ
enum class TextureFilterType 
{
	Nearest = 0,	//! ニアレスト補間
	Linear = 1,		//! バイリニア補間
};

/// @brief テクスチャのワープタイプ
enum class TextureWrapType
{
	Repeat = 0,		//! 繰り返し
	Clamp = 1,		//! 打ち切り
};

enum class CullingType
{
	Frond = 0,
	Back = 1,
	Double = 2,
};

///	@brief レンダーステートを集約した構造体
struct RenderState
{
	bool				depthTest	: 1;	//! 深度テストを行うか	
	bool				depthWrite	: 1;	//! 深度バッファへの書き込みを行うか
	AlphaBlendType		alphaBlend;			//! アルファブレンド
	CullingType			cullingType;		//! カリングタイプ
	TextureFilterType	textureFilterType;	//! テクスチャフィルタタイプ
};
