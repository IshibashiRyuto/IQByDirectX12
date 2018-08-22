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

/// @class TextureLoader
/// @brief テクスチャの読み込みを行うクラス
/// このクラスはテクスチャデータを読み込み、
/// テクスチャデータの管理を行う。
/// すでに読み込み済みのテクスチャを読み込む際は
/// 読み込み済みデータを返すようにする
class TextureLoader
{
public:
	/// コンストラクタ
	TextureLoader();

	/// デストラクタ
	~TextureLoader();

	/// @fn Create
	/// テクスチャローダーを生成する
	/// @note TextureLoaderクラスはこのメソッドを通じてのみ生成可能
	/// 実体を直接持つことはできない
	static std::shared_ptr<TextureLoader> Create(ComPtr<ID3D12Device> device);

	std::shared_ptr<Texture> Load(const std::string& str);
private:
	/*変数宣言*/
	std::map< std::string, std::shared_ptr<Texture>> mTextureResource;
	ComPtr<ID3D12Device> mDevice;

	/*ローカルメソッド*/

	/// @fn GetWString
	/// 文字列をロング文字列に変換する
	/// @param[in]	1バイト文字列
	/// @retval		wstring
	std::wstring GetWString(const std::string& str);

	/// @fn UpdateTextureSubresource
	/// テクスチャのサブリソースをアップデートする
	/// @param[in]	resource ID3D12Resourceのポインタ
	/// @param[in]	subresource サブリソースデータ
	void UpdateTextureSubresource(ComPtr<ID3D12Resource> resource, D3D12_SUBRESOURCE_DATA& subresource);
};

