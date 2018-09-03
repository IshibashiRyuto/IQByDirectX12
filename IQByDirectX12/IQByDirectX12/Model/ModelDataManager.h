/*
	ModelDataManager.h
	モデルデータ管理クラス
	@author Ishibashi Ryuto
	@date
	2018/09/03	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <map>

// 自作ヘッダインクルード
#include "ModelData.h"

class ModelDataManager
{
public:
	~ModelDataManager();

	/// @fn static GetInstance
	/// シングルトンインスタンスを取得する
	/// @retval モデルマネージャのインスタンス
	static ModelDataManager& GetInstance()
	{
		static ModelDataManager inst;
		return inst;
	}

	/// @fn Regist
	/// モデルデータを登録し、管理ハンドルを返す
	/// @param[in] modelData モデルデータ
	/// @retval		モデルデータのハンドル
	int Regist(std::shared_ptr<ModelData> modelData);

	/// @fn Erase
	/// 指定したハンドルのモデルデータを削除する
	/// @param[in] handle モデルハンドル
	void Erase(int handle);
	
	/// @fn GetModelData
	/// モデルデータを作成する
	/// @param[in] handle	: モデルハンドル
	/// @retval モデルデータ
	std::shared_ptr<ModelData> GetModelData(int handle) const;

	/// @fn IsExist
	/// 指定したハンドルを指すモデルデータが存在するが確認する
	/// @param[in] handle : テクスチャハンドル
	/// @retval 存在する: true
	/// @retval 存在しない: false
	bool IsExist(int handle) const;

private:
	ModelDataManager();
	ModelDataManager(const ModelDataManager&) {}
	void operator=(const ModelDataManager &){}

	/* 定数定義 */
	const int DATA_SIGNATURE_SHIFT_NUM = 24;
	const int MODEL_DATA_SIGNATURE = 0x11 << DATA_SIGNATURE_SHIFT_NUM;

	/* 変数宣言 */
	std::map<int, std::shared_ptr<ModelData>> mData;		// 管理データ
	int mNextHandle;									// 次に使用するテクスチャハンドル


	/* ローカルメソッド宣言 */
	/// @fn 次に使用するハンドルを探索し、更新する
	void UpdateNextHandle();
};

