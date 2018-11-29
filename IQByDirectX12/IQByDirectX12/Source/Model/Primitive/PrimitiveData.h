/**
*	@file	PrimitiveData.h
*	@brief	プリミティブモデルに関する情報を記述する
*	@author	IshibashiRyuto
*	@date	2018/11/28	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "../Math/Math.h"

/**
*	@namespace	Primitive
*	@brief	プリミティブに関する情報をまとめた名前空間
*/
namespace Primitive
{
	/**
	*	@struct	Vertex
	*	@brief	頂点情報
	*/
	struct Vertex
	{
		Math::Vector3 pos;		//! 位置情報
		Math::Vector3 normal;	//! 法線
		Math::Vector2 uv;		//! テクスチャマッピング用UV
		
		/**
		*	@brief	コンストラクタ
		*/
		Vertex();

		/**
		*	@brief	コンストラクタ
		*	
		*	@param[in]	pos		: 位置情報
		*	@param[in]	normal	: 法線情報
		*	@param[in]	uv		: uv情報
		*/
		Vertex(const Math::Vector3& pos, const Math::Vector3& normal, const Math::Vector2 & uv);
	};

	/**
	*	@struct	Material
	*	@brief	マテリアル情報
	*/
	struct Material
	{
		Math::Vector4 diffuse;	//!	ディフューズ
		Math::Vector3 specular; //! スペキュラーカラー
		float specularity;		//! スペキュラ強さ(累乗係数)
		Math::Vector3 ambient;	//! アンビエントカラー

		/**
		*	@brief	コンストラクタ
		*/
		Material();

		/**
		*	@brief	コンストラクタ
		*
		*	@param[in]	diffuse		: ディフューズカラー
		*	@param[in]	ambient		: アンビエントカラー
		*	@param[in]	specular		: スペキュラーカラー
		*	@param[in]	specularity	: スペキュラ強さ(累乗係数)
		*/
		Material(const Math::Vector4& diffuse, const Math::Vector3& ambient, const Math::Vector3& specular, float specularity);
	};

	/**
	*	@typedef	Index
	*	@brief		インデックスデータ
	*/
	typedef short Index;
}