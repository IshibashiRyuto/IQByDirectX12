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
	/// @retval int :		テクスチャ管理ハンドル
	int Load(const std::string& filePath);
	int Load(const std::wstring& filePath);
private:
	/*変数宣言*/
	std::map< std::wstring, int> mTextureHandleManager;			// 読み込み済みテクスチャのハンドルを管理する
	std::shared_ptr<Device> mDevice;								// デバイス
	TextureManager &mTextureManager;							// テクスチャマネージャへの参照
	std::shared_ptr<GraphicsCommandList> mCommandList;			// コマンドリスト
	std::shared_ptr<CommandQueue>	mCommandQueue;				// コマンドキュー

	ComPtr<ID3D12Resource> mUpdateBuffer;						// UpdateSubResourceで使用

	/*ローカルメソッド*/

	/// コンストラクタ
	TextureLoader(std::shared_ptr<Device> device);

	/// @fn UpdateTextureSubresource
	/// テクスチャのサブリソースをアップデートする
	/// @param[in]	resource ID3D12Resourceのポインタ
	/// @param[in]	subresource サブリソースデータ
	void UpdateTextureSubresource(ComPtr<ID3D12Resource> resource, D3D12_SUBRESOURCE_DATA& subresource);
};

