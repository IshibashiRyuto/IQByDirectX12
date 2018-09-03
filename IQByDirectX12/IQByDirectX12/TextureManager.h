/*
	TextureManager.h
	テクスチャ管理クラス
	2018/09/02 初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <map>

// 自作ヘッダインクルード
#include "Texture.h"

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
	/// @retval		テクスチャのハンドル
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
	/// @retval true: 存在する
	/// @retval false: 存在しない
	bool IsExist(int handle) const;
	
private:
	TextureManager();
	TextureManager(const TextureManager&) {}
	void operator=(const TextureManager&) {}

	std::map<int, std::shared_ptr<Texture>> mData;
	int mNextTextureHandle;

	/// @fn UpdateNowTextureHandle
	/// 次のテクスチャハンドルを探索し、更新する
	void UpdateNextTextureHandle();

	const int DATA_SIGNATURE_SHIFT_NUM = 24;
	const int TEXTURE_SIGNATURE = 0x10 << DATA_SIGNATURE_SHIFT_NUM;
};

