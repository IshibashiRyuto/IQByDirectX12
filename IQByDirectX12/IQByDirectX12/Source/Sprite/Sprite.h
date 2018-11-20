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

/* クラス使用宣言 */
class Texture;
class InstancingDataManager;
class Device;
class PipelineStateObject;

/**	@class	Sprite
*	@brief	2Dスプライトクラス
*
*	2Dオブジェクトであるスプライトデータを格納するクラス
*	位置指定、回転指定、倍率指定等設定して描画可能
*	2Dデータであるため、2次元以外の情報を持たないため注意
*	3D変換として取り扱うことも不可
*	3次元スプライトを取り扱う場合にはPlaneクラスを使用すること
*/
class Sprite
{
public:
	/**
	*	@brief	コンストラクタ
	*/
	Sprite(int spriteHandle);

	/**
	*	@brief	デストラクタ
	*/
	~Sprite();

	/**
	*	@brief	スプライト作成時に使用するパイプラインステートオブジェクトをセットする
	*	@param[in]	pso	: パイプラインステートオブジェクト
	*/
	static void SetPipelineStateObject(std::shared_ptr<PipelineStateObject> pso);

	/**
	*	@brief	スプライトを作成する
	*	@param[in] texture	: テクスチャ
	*	@retval	スプライトのスマートポインタ
	*	@note	テクスチャサイズのスプライトを作成する
	*/
	static std::shared_ptr<Sprite> Create(std::shared_ptr<Device> device, std::shared_ptr<Texture> texture);
	


	/**
	*	@brief	スプライトを描画する
	*/
	void Draw() const;

	/**
	*	@brief	スプライトの描画位置を設定する
	*/
	void SetPosition(float x, float y);
	void SetPosition(const Math::Vector2& pos);

	/**
	*	@brief	スプライトの回転角度を設定する
	*/
	void SetRotation(float rad);

	/**
	*	@brief	スプライトの拡大率を設定する
	*/
	void SetScale(float scale);
	void SetScale(const Math::Vector2& scale);

	/**
	*	@brief	スプライトに張り付けるテクスチャを変更する
	*/
	void SetTexture(std::shared_ptr<Texture> texture);

	/**
	*	@brief	スプライトを複製する
	*
	*	現在のスプライト情報を用いて、スプライトを複製する
	*	複製されたスプライトは個別の位置、回転角、スケールを持つが、
	*	頂点サイズとテクスチャは共有される
	*	複製したほうが描画効率は良くなるが、テクスチャを差し替えると
	*	すべてのテクスチャが差し変わるため注意すること
	*/
	std::shared_ptr<Sprite> Duplicate();

private:
	//パイプラインステートオブジェクト
	static std::shared_ptr<PipelineStateObject> sPSO;

	int mSpriteHandle;						//! スプライトデータのハンドル
	std::shared_ptr<Texture> mTexture;		//! テクスチャ
	Math::Vector2 mPos;						//! 描画位置
	float mAngle;							//! 回転角度
	Math::Vector2 mScale;					//! 拡大率
	Math::Matrix3x3 mInstMat;				//! インスタンス行列
	InstancingDataManager& mInstDataMgr;	//! インスタンス用データマネージャへの参照

	/**
	*	@brief	現在の位置、回転、拡縮から行列を計算する
	*/
	void CalcMatrix();
};

