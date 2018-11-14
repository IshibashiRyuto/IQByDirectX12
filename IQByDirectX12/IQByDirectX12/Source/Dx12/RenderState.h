/*
*	@file RenderState.h
*	@brief レンダーステート情報をまとめたファイル
*	@author Ishibashi Ryuto
*	@date
*	2018/09/26 初版作成
*/
#pragma once

/* ヘッダインクルード */
#include <d3d12.h>
#include <map>

const int ALPHA_TYPE_COUNT = 5;
/// @brief アルファブレンドオプション
enum class AlphaBlendType
{
	Opacity = 0,	//! 不透明
	Blend = 1,		//! 透明
	Add = 2,		//! 加算
	Sub = 3,		//! 減算
	Mul = 4,		//! 乗算
};

/// @brief サンプリングする際のサンプリング方法
enum class TextureFilterType 
{
	Nearest = 0,	//! ニアレスト補間
	Linear = 1,		//! バイリニア補間
};

/// @brief UV値が0以下もしくは1以上の場合の扱い方
enum class TextureWrapType
{
	Repeat = 0,		//! 繰り返し
	Clamp = 1,		//! 打ち切り
};

const int CULLING_TYPE_COUNT = 3;
enum class CullingType
{
	Front = 0,
	Back = 1,
	Double = 2,
};

///	@class	RenderState
///	@brief	レンダーステートを集約したクラス
///
///	レンダリング情報を集約したクラス
/// 設定した情報をもとに、PipelineStateDescを構築して返すメソッドを持つ
class RenderState
{
public:
	bool				depthTest	: 1;	//! 深度テストを行うか	
	bool				depthWrite	: 1;	//! 深度バッファへの書き込みを行うか
	AlphaBlendType		alphaBlendType;			//! アルファブレンド
	CullingType			cullingType;		//! カリングタイプ
	TextureFilterType	textureFilterType;	//! サンプリングする際のサンプリング方法
	TextureWrapType		textureWrapType;	//! UV値が0以下もしくは1以上の場合の扱い方
	D3D12_COMPARISON_FUNC depthFunc;		//! デプスの比較関数
	/// @brief	現在設定された情報からパイプラインステートデスクを構築して取得する
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GetPipelineStateDesc() const;
	
private:
	static bool mIsInitialize;				//! 初期化済みか
	static std::map<CullingType, D3D12_RASTERIZER_DESC>	mRasterizerStates;					//! ラスタライザステート
	static std::map<bool, std::map<bool, D3D12_DEPTH_STENCIL_DESC>>	mDepthStencilStates;	//! デプスステンシルステート
	static std::map<AlphaBlendType, D3D12_BLEND_DESC>	mBlendStates;						//! ブレンドステート
	
	/// @brief	ステート情報テーブルの初期化
	void InitStateTable() const;
};
