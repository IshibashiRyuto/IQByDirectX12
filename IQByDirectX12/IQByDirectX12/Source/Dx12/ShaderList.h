/*
*	@file	ShaderList.h
*	@brief	PSOにセットするシェーダリスト構造体を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/14	初版作成
*/
#pragma once

#include "../Shader.h"


/// @struct	ShaderList
/// @brief	PSOに投げるシェーダリスト
struct ShaderList
{
	std::shared_ptr<Shader> VS;		//! バーテックスシェーダ
	std::shared_ptr<Shader> PS;		//! ピクセルシェーダ
	std::shared_ptr<Shader> GS;		//! ジオメトリシェーダ
	std::shared_ptr<Shader> DS;		//! ドメインシェーダ
	std::shared_ptr<Shader> HS;		//! ハルシェーダ
};
