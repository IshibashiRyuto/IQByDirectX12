/*
	TextureManager.h
	テクスチャ管理クラス
	@author Ishibashi Ryuto
	@date	2018/09/02 初版作成
			2018/11/27	初期化処理とテクスチャ更新処理を追加
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <map>

// 自作ヘッダインクルード
#include "../Dx12/Buffer/Texture.h"

// クラス使用宣言
class Device;
class CommandQueue;

class TextureManager
{
public:
	~TextureManager();

	/// @fn static GetInstance
	/// シングルトンインスタンスを取得する
	/// @retval テクスチャマネージャのインスタンス
	static TextureManager& GetInstance()
	{
		static TextureManager inst;
		return inst;
	}

	/// @fn Regist
	/// テクスチャデータをマネージャに登録し、管理ハンドルを返す
	/// @param[in] texture : テクスチャデータ
	/// @return		テクスチャのハンドル
	int Regist(std::shared_ptr<Texture> texture);

	/// @fn Erase
	/// 指定したハンドルのテクスチャを削除する
	/// @param[in] int テクスチャハンドル
	void Erase(int textureHandle);

	/// @fn GetTexture
	/// テクスチャデータを取得する
	/// @param[in] textureHandle : テクスチャハンドル
	/// @retval テクスチャデータ
	std::shared_ptr<Texture> GetTexture(int textureHandle) const;

	/// @fn IsExist
	/// 対象としたハンドルの先が存在するか確認する
	/// @param[in] handle テクスチャハンドル
	/// @retval 存在する: true
	/// @retval 存在しない: false
	bool IsExist(int handle) const;

	/**
	*	@brief	テクスチャ情報を更新する
	*
	*	テクスチャ情報の書き込み(通常のテクスチャ読み込みも含む)は
	*	一時リソースに対して行われており、実際の書き込みは
	*	このメソッドを呼び出して行う必要がある。
	*/
	void UpdateTextureData();

	/**
	*	@brief		初期化処理
	*
	*	@param[in]	device	: dx12デバイス
	*
	*	@return		初期化に成功したか
	*
	*	@note		テクスチャのロード処理よりも先に行う必要がある
	*/
	bool Initialize(std::shared_ptr<Device> device);

	static const int WHITE_TEXTURE = -1;
	static const int BLACK_TEXTURE = -2;
	
private:
	TextureManager();
	TextureManager(const TextureManager&) {}
	void operator=(const TextureManager&) {}

	std::map<int, std::shared_ptr<Texture>> mData;		//! テクスチャデータ
	int mNextTextureHandle;								//! 次に使用するテクスチャハンドル
	std::shared_ptr<GraphicsCommandList> mCommandList;	//! 画像更新用コマンドリスト
	std::shared_ptr<CommandQueue> mCommandQueue;		//! 画像更新用コマンドキュー
	bool mIsInit;										//! 初期化したか

	/// @fn UpdateNowTextureHandle
	/// 次のテクスチャハンドルを探索し、更新する
	void UpdateNextTextureHandle();

	/// @brief ヌルテクスチャを作成する
	void CreateWhiteAndBlackTexture(std::shared_ptr<Device> device);

	static const int DATA_SIGNATURE_SHIFT_NUM = 24;
	static const int TEXTURE_SIGNATURE = 0x10 << DATA_SIGNATURE_SHIFT_NUM;
};

