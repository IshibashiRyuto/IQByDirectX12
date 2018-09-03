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

// 自作ヘッダインクルード

// クラス使用宣言
class VertexBuffer;

// 構造体定義
struct InstanceData
{
	int maxInstanceCount;
	int nowInstanceCount;
	size_t dataSize;
	std::shared_ptr<VertexBuffer> vertexBuffer;
};

class InstancingDataManager
{
public:
	// コンストラクタ
	InstancingDataManager();

	// デストラクタ
	~InstancingDataManager();

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
	/// @param[in] maxInstanceCount	: 最大インスタンス数
	/// @note 描画中に呼び出した場合、データを削除するため注意
	void ResetMaxInstanceCount(int handle, int maxInstanceCount = 1);

	const InstanceData& GetInstanceData(int handle);

private:
	std::map<int, InstanceData> mInstanceDataMap;
};

