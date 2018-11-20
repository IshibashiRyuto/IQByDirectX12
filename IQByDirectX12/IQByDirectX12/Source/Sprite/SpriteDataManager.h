/**
*	@file	SpriteDataManager.h
*	@brief	スプライトデータマネージャを記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/20	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include <memory>
#include <map>

/* クラス使用宣言 */
class SpriteData;
class GraphicsCommandList;
class InstancingDataManager;

/**
*	@class	SpriteDataManager
*	@biref	スプライトデータを管理するシングルトンクラス
*/
class SpriteDataManager
{
public:
	~SpriteDataManager();

	/**
	*	@brief	シングルトンインスタンスの取得
	*	@retval	スプライトデータマネージャのインスタンス
	*/
	static SpriteDataManager& GetInstance()
	{
		static SpriteDataManager inst;
		return inst;
	}

	/**
	*	@brief	スプライトデータを登録し、管理ハンドルを返す
	*	@param[in]	spriteData	: スプライトデータ
	*	@retval		スプライトデータのハンドル
	*/
	int Regist(std::shared_ptr<SpriteData> spriteData);

	/**
	*	@brief	指定したハンドルのスプライトデータを削除する
	*	@param[in]	handle	: スプライトハンドル
	*/
	void Erase(int handle);

	/**
	*	@brief	指定したハンドルのスプライトデータが存在するかチェックする
	*	@param[in]	handle	: ハンドル
	*	@retval	存在する	: true
	*	@retval	存在しない	: false
	*/
	bool IsExist(int handle) const;

	/**
	*	@brief	管理しているスプライトデータを描画する
	*	@param[in]	commandList	: コマンドリスト
	*/
	void Draw(std::shared_ptr<GraphicsCommandList> commandList);

private:
	/* 定数定義 */
	const int DATA_SIGNATURE_SHIFT_NUM = 24;
	const int SPRITE_DATA_SIGNATURE = 0x12 << DATA_SIGNATURE_SHIFT_NUM;

	/* 変数宣言 */
	std::map<int, std::shared_ptr<SpriteData>> mData;		//! 管理データ
	int mNextHandle;										//! 次に使用するスプライトデータハンドル
	InstancingDataManager& mInstancingDataManager;			//! インスタンシングデータ管理クラスへの参照

	/* ローカルメソッド定義 */
	/**
	*	@brief	次に使用するハンドルを探索し、更新する
	*/
	void UpdateNextHandle();

	/**
	*	@brief	コンストラクタ
	*/
	SpriteDataManager();

	/**
	*	@brief	コピーコンストラクタ
	*/
	SpriteDataManager(const SpriteDataManager&);

	/**
	*	@brief	=演算子でのコピー禁止
	*/
	void operator=(const SpriteDataManager&) {}
};

