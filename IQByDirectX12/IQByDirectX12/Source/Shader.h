/*
	Shader.h
	シェーダ情報を管理するクラス
	@author Ishibashi Ryuto
	@date
	2018/09/06	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <string>
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// 自作ヘッダインクルード

// クラス使用宣言
using Microsoft::WRL::ComPtr;

class Shader
{
public:
	/// デストラクタ
	~Shader();

	/// @fn Create
	/// シェーダクラスを生成する
	/// @note Shaderクラスはこのメソッドを通じてのみ生成可能
	/// @param[in] filePath		:	シェーダファイルパス
	/// @param[in] entryPoint	:	シェーダエントリポイント
	/// @param[in] target		:	ターゲット
	///	@retval		生成成功	:	Shaderのポインタ
	/// @retval		生成失敗	:	nullptr
	static std::shared_ptr<Shader> Create(const std::wstring& filePath, const std::string& entryPoint, const std::string& target);

	/// @fn GetShaderByteCode
	/// シェーダコードを取得する
	/// @retval シェーダバイトコード
	const D3D12_SHADER_BYTECODE & GetShaderByteCode() const;

	/// @fn LoadShader
	/// シェーダをロードする
	void LoadShader(const std::wstring & filePath, const std::string& entryPoint, const std::string& compileTarget);

private:
	/* メンバ変数定義 */
	ComPtr<ID3DBlob> mShaderCode;
	D3D12_SHADER_BYTECODE mShaderByteCode;

	/* ローカルメソッド定義 */
	/// コンストラクタ
	Shader();

};

