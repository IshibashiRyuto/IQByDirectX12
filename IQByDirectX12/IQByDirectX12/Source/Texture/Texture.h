/*
	Texture.h
	テクスチャデータを扱うクラス
	@author Ishibashi Ryuto
	@param history
	2018/07/28 初版作成
*/
#pragma once
/*システムヘッダインクルード*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*自作ヘッダインクルード*/

/*前方宣言*/
using Microsoft::WRL::ComPtr;

class Texture
{
public:
	/// デストラクタ
	virtual ~Texture();

	/// @fn Create
	/// テクスチャデータを生成する
	/// @note Textureクラスはこのメソッドを通じてのみ生成可能
	/// 実体を直接持つことはできない
	/// @param[in] textureData :テクスチャデータ書き込み済みのリソースバッファ
	/// @retval 生成成功: Textureのshared_ptr
	/// @retval 生成失敗: nullptr
	static std::shared_ptr<Texture> Create(ComPtr<ID3D12Resource> textureData);

	/// @fn GetTextureData
	/// テクスチャデータを取得する
	/// @retval	テクスチャデータ
	const ComPtr<ID3D12Resource> GetTextureData() const;

	/// @fn GetShaderResourceView
	/// シェーダーリソースビューを取得する
	/// @retval シェーダリソースビュー
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetShaderResourceView() const;

	/// @fn SetShaderResourceView
	/// シェーダーリソースビューを設定する
	void SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srv);

protected:
	ComPtr<ID3D12Resource> mTextureData;
	D3D12_SHADER_RESOURCE_VIEW_DESC mShaderResourceView;


	/// コンストラクタ
	Texture(ComPtr<ID3D12Resource> textureData);

	/// リソース情報を元にSRVを構築する
	void ConstructSRV();
};
