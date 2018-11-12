/*
*	@file	Texture.h
*	@brief	テクスチャデータを扱うクラス
*	@author Ishibashi Ryuto
*	@date	2018/07/28 初版作成
*/
#pragma once
/*ヘッダインクルード*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*クラス使用宣言*/
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

	/// @fn GetShaderResourceViewDesc
	/// シェーダーリソースビューを取得する
	/// @retval シェーダリソースビュー
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetShaderResourceViewDesc() const;

	/// @fn SetShaderResourceViewDesc
	/// シェーダーリソースビューを設定する
	void SetShaderResourceViewDesc(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc);

protected:
	ComPtr<ID3D12Resource> mTextureData;
	D3D12_SHADER_RESOURCE_VIEW_DESC mShaderResourceViewDesc;


	/// コンストラクタ
	Texture(ComPtr<ID3D12Resource> textureData);

	/// リソース情報を元にSRVを構築する
	void ConstructSRVDesc();
};
