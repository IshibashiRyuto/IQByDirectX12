/*
	ModelDataManager.h
	モデルデータ管理クラス
	@author Ishibashi Ryuto
	@date
	2018/09/03	初版作成
	2018/11/27	DrawNoMat追加
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <map>

// 自作ヘッダインクルード
#include "ModelData.h"

// クラス使用宣言
class InstancingDataManager;

class ModelDataManager
{
public:
	~ModelDataManager();

	/// シングルトンインスタンスを取得する
	/// @retval モデルマネージャのインスタンス
	static ModelDataManager& GetInstance()
	{
		static ModelDataManager inst;
		return inst;
	}

	/// モデルデータを登録し、管理ハンドルを返す
	/// @param[in] modelData モデルデータ
	/// @retval		モデルデータのハンドル
	int Regist(std::shared_ptr<ModelData> modelData);

	/// 指定したハンドルのモデルデータを削除する
	/// @param[in] handle モデルハンドル
	void Erase(int handle);
	
	/// モデルデータを取得する
	/// @param[in] handle	: モデルハンドル
	/// @retval モデルデータ
	std::shared_ptr<ModelData> GetModelData(int handle) const;

	/// 指定したハンドルを指すモデルデータが存在するが確認する
	/// @param[in] handle : テクスチャハンドル
	/// @retval 存在する: true
	/// @retval 存在しない: false
	bool IsExist(int handle) const;

	/// 管理しているモデルデータを描画する
	/// @param[in]	graphicsCommandList	: 描画先コマンドリスト
	///	@param[in]	isReset				: 描画後、描画記録をリセットするか(default:true)
	void Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, bool isReset = true);

	/**
	*	@brief	マテリアルを使用せずに描画する
	*
	*	@param[in]	graphicsCommandList	: 描画先コマンドリスト
	*	@param[in]	isReset				: 描画後に描画記録をリセットするか(default: true)
	*/
	void DrawNoMat(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, bool isReset = true);

private:
	ModelDataManager();
	ModelDataManager(const ModelDataManager&);
	void operator=(const ModelDataManager &){}

	/* 定数定義 */
	const int DATA_SIGNATURE_SHIFT_NUM = 24;
	const int MODEL_DATA_SIGNATURE = 0x11 << DATA_SIGNATURE_SHIFT_NUM;

	/* 変数宣言 */
	std::map<int, std::shared_ptr<ModelData>> mData;		// 管理データ
	int mNextHandle;									// 次に使用するテクスチャハンドル
	InstancingDataManager& mInstancingDataManager;		// インスタンシング用データ管理クラスへの参照


	/* ローカルメソッド宣言 */
	/// @fn 次に使用するハンドルを探索し、更新する
	void UpdateNextHandle();
};

