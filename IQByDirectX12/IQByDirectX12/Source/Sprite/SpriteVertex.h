/**
*	@file	SpriteVertex.h
*	@brief	スプライトの頂点情報を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/20	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "../Math/Math.h"

/**
*	@struct	SpriteVertex
*	@brief	スプライトの頂点情報
*/
struct SpriteVertex
{
	Math::Vector4 pos;	//! 頂点座標
	Math::Vector2 uv;	//! UV座標(テクスチャ貼り付け用)
};
