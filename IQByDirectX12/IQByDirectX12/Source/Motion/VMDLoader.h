/*
*	@file VMDLoader.h
*	@brief VMDデータを読み込むクラス
*	@author Ishibashi Ryuto
*	@date
*	2018/09/25	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <string>

//クラス使用宣言
class VMDData;
class Animation;


class VMDLoader
{
public:
	VMDLoader();
	~VMDLoader();

	/// @brief VMDLoaderインスタンスを生成する
	/// @note このクラスのインスタンスはこのメソッドを通じてのみ生成可能
	static std::shared_ptr<VMDLoader> Create();

	/// @brief VMDDataをロードする
	/// @param[in] filePath	:	ロードするファイルのパス
	/// @retval	読み込み成功	: アニメーションのスマートポインタ
	/// @retval 読み込み失敗	: nullptr
	std::shared_ptr<Animation> Load(const std::string& filePath);
};

