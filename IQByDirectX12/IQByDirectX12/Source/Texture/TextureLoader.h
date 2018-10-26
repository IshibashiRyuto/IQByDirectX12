/*
	TextureLoader.h
	テクスチャーローダー
	@author Ishibashi Ryuto
	@param history
	2018/07/28 初版作成
*/
#pragma once

/*システムヘッダインクルード*/
#include <string>
#include <memory>
#include <map>
#include <DirectXTex.h>

/*自作ヘッダインクルード*/
#include "Texture.h"
#include "TextureManager.h"

/*クラス仕様宣言*/
class CommandQueue;
class GraphicsCommandList;
class Device;

/// @class TextureLoader
/// @brief テクスチャの読み込みを行うクラス
/// このクラスはテクスチャデータを読み込み、
/// テクスチャデータの管理を行う。
/// すでに読み込み済みのテクスチャを読み込む際は
/// 読み込み済みデータを返すようにする
class TextureLoader
{
public:
	/// デストラクタ
	~TextureLoader();

	/// @fn Create
	/// テクスチャローダーを生成する
	/// @note TextureLoaderクラスはこのメソッドを通じてのみ生成可能
	/// 実体を直接持つことはできない
	/// @param[in] device　：デバイス
	static std::shared_ptr<TextureLoader> Create(std::shared_ptr<Device> device);

	/// @fn Load
	/// テクスチャをロードし、テクスチャのハンドルを返す
	/// @param[in] filePath : テクスチャのファイルパス
	/// @retval 読み込み成功時 : テクスチャ管理ハンドル
	/// @retval 読み込み失敗時 : -1
	int Load(const std::string& filePath);
	int Load(const std::wstring& filePath);
private:
	/* 変数宣言 */
	std::map< std::wstring, int> mTextureHandleManager;			// 読み込み済みテクスチャのハンドルを管理する
	std::shared_ptr<Device> mDevice;								// デバイス
	TextureManager &mTextureManager;							// テクスチャマネージャへの参照

	ComPtr<ID3D12Resource> mUpdateBuffer;						// UpdateSubResourceで使用

	/*ローカルメソッド*/

	/// コンストラクタ
	TextureLoader(std::shared_ptr<Device> device);

	/// @fn LoadTGATexture
	/// @brief		TGAテクスチャを読み込む
	/// @param[in]	filePath	: ファイルパス
	/// @param[out]	metaData	: メタデータ
	/// @param[out] image		: イメージデータ
	/// @retval		読み込み成功の可否
	bool LoadTGATexture(std::wstring filePath, DirectX::TexMetadata* metaData, DirectX::ScratchImage& imageData) const;

	/// @fn LoadDDSTexture
	/// @brief		DDSテクスチャを読み込む
	/// @param[in]	filePath	: ファイルパス
	/// @param[out]	metaData	: メタデータ
	/// @param[out] image		: イメージデータ
	/// @retval		読み込み成功の可否
	bool LoadDDSTexture(std::wstring filePath, DirectX::TexMetadata* metaData, DirectX::ScratchImage& imageData) const;

	/// @fn LoadWICTexture
	/// @brief		WICテクスチャを読み込む
	/// @param[in]	filePath	: ファイルパス
	/// @param[out]	metaData	: メタデータ
	/// @param[out] image		: イメージデータ
	/// @retval		読み込み成功の可否
	bool LoadWICTexture(std::wstring filePath, DirectX::TexMetadata* metaData, DirectX::ScratchImage& imageData) const;

	/// @fn CreateTextureResource
	/// @brief テクスチャリソースを作成する
	/// 受け取ったメタデータとイメージデータをもとに
	/// テクスチャリソースを作成する
	/// @param[in]	metaData	: メタデータ
	/// @param[in]	image		: イメージデータ
	/// @retval		テクスチャリソースへのComポインタ
	ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& imageData);
};

