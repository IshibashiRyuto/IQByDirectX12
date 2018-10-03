/*
	InstansingDataManager.h
	インスタンシングに必要な情報を管理する
	@author Ishibashi Ryuto
	@date
	2018/09/03 初版作成
*/
#pragma once
// システムヘッダインクルード
#include <map>
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// 自作ヘッダインクルード

// クラス使用宣言
class InstanceBuffer;
using Microsoft::WRL::ComPtr;

// 構造体定義
struct InstanceData
{
	int maxInstanceCount;
	int nowInstanceCount;
	size_t dataSize;
	std::shared_ptr<InstanceBuffer> instanceBuffer;
};

class InstancingDataManager
{
public:
	// デストラクタ
	~InstancingDataManager();

	/// @fn GetInstance
	/// シングルトンインスタンスを取得する
	/// @retval const InstancingDataManager& : InstancingDataManagerクラスの参照
	static InstancingDataManager& GetInstance()
	{
		static InstancingDataManager inst;
		return inst;
	}

	/// @fn SetDevice
	/// デバイスをセットする
	/// @param[in] device	: ID3D12デバイス
	void SetDevice(ComPtr<ID3D12Device> device);

	/// @fn SetInstanceData
	/// インスタンシング用のデータをセットする
	/// @param[in] handle	: インスタンシング対象のハンドル
	/// @param[in] data		: データのポインタ
	/// @param[in] size		: データのサイズ
	/// @note インスタンシング登録していない場合、データの登録を行わない。
	void SetInstanceData(int handle, void* data, size_t size);

	/// @fn ResetMaxInstanceCount
	/// 指定したハンドルの最大インスタンス数を修正する
	/// @param[in] handle			: 修正対象のハンドル
	/// @param[in] instanceDataSize : インスタンスデータ1つあたりのサイズ
	/// @param[in] maxInstanceCount	: 最大インスタンス数
	/// @note 描画中に呼び出した場合、データを削除するため注意
	void ResetMaxInstanceCount(int handle, size_t instanceDataSize, int maxInstanceCount = 1);

	/// @fn ResetInstanceData
	/// インスタンスデータをリセットする
	/// @param[in] handle	: リセット対象のハンドル
	void ResetInstanceData(int handle);

	/// @fn ResetInstanceDataAll
	/// 登録されているインスタンスデータすべてをリセットする
	void ResetInstanceDataAll();

	/// @fn GetInstanceDataAll
	/// 登録されているインスタンスデータすべてを取得する
	/// @retval インスタンスデータとハンドルのマップ
	const std::map<int, InstanceData>& GetInstanceDataAll();

private:
	/* メンバ変数定義 */
	std::map<int, InstanceData> mInstanceDataMap;		// インスタンシングデータ管理用マップ
	ComPtr<ID3D12Device> mDevice;						// ID3D12デバイス

	/* ローカルメソッド定義 */
	// コンストラクタ
	InstancingDataManager();
};

