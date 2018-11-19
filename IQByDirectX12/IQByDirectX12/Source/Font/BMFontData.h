/**
*	@file	BitMapFontData.h
*	@brief	フォントデータを記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/19	初版作成
*
*	BMFontで作成したフォントデータのデータ構造
*	フォントデータは5つのブロックから成る
*	参考:http://www.angelcode.com/products/bmfont/doc/file_format.html
*/
#pragma once
/* ヘッダインクルード */
#include <string>
#include <vector>

/**
*	@namespace	BMFont
*	@brief		BMFont関連のデータおよびメソッドを記述する
*/

namespace BMFont
{

	/**
	*	@enum	BlockType
	*	@brief	ビットマップフォントのブロックタイプ
	*/
	enum class BlockType
	{
		Info = 1,
		Common,
		Pages,
		Chars,
		KerningPair
	};

	/**
	*	@struct Info
	*	@brief	BMFontのInfoブロックに記述される情報
	*/
	struct Info
	{
		short fontSize;	//! フォントサイズ
		union
		{
			struct
			{
				bool reserved : 3;		//!	予約領域
				bool fixedHeight : 1;	//!	高さ修正があるか
				bool bold : 1;			//! ボールド体か
				bool italic : 1;		//! イタリック体か
				bool unicode : 1;		//! Unicodeを使用しているか
				bool smooth : 1;		//! スムースフォントか
			};
			char bitField;	//!	ビットフィールド
		};
		unsigned char charSet;			//! 文字セット
		unsigned short stretchH;			//! 高さストレッチ
		unsigned char aa;				//! アンチエイリアス
		unsigned char paddingUp;		//! 文字列のパディング距離(上)
		unsigned char paddingRight;		//! 文字列のパディング距離(右)
		unsigned char paddingDown;		//! 文字列のパディング距離(下)
		unsigned char paddingLeft;		//! 文字列のパディング距離(左)
		unsigned char spacingHoriz;		//! 水平方向の文字間隔
		unsigned char spacingVert;		//! 垂直方向の文字間隔
		unsigned char outline;			//! アウトライン
		std::string fontName;			//! フォント名
		std::wstring wstringFontName;	//! フォント名(wstring版)
	};

	/**
	*	@struct	Common
	*	@brief	BMFontのCommonブロックに記述される情報
	*/
	struct Common
	{
		unsigned short lineHeight;		//! テキスト1行当たりの高さ(pixel)
		unsigned short base;			//! 行の上端からベースラインまでの距離(pixel)
		unsigned short scaleW;			//! フォントテクスチャ1枚当たりの横幅(pixel)
		unsigned short scaleH;			//! フォントテクスチャ1枚当たりの縦幅(pixel)
		unsigned short pages;			//! ページの数
		union
		{
			char bitField;				//! ビットフィールド
			struct
			{
				bool packed : 1;		//! パックされているか
				bool reserved : 7;		//! 予約領域
			};
		};
		unsigned char alphaChnl;		//! アルファチャネルの情報
		unsigned char redChnl;			//! 赤チャネルの情報
		unsigned char greenChnl;		//! 緑チャネルの情報
		unsigned char blueChnl;			//! 青チャネルの情報
	};

	/**
	*	@struct Pages
	*	@brief	BMFontのPagesブロックに記述される情報
	*/
	struct Pages
	{
		std::vector<std::string> pageNames;			//! ページ名リスト
		std::vector<std::wstring> wstringPageNames;	//! ページ名リスト(wstinrg版)	
	};

	/**
	*	@struct CharData
	*	@brief	BMFontのCharsブロックに記述される、1文字当たりの情報
	*/
	struct CharData
	{
		unsigned int id;			//! 文字コード
		unsigned short x;			//! 画像の切り取り位置(x座標)
		unsigned short y;			//! 画像の切り取り位置(y座標)
		unsigned short width;		//! 画像の切り取り幅
		unsigned short height;		//! 画像の切り取り高さ
		unsigned short xOffset;		//! 描画オフセット(x座標)
		unsigned short yOffset;		//! 描画オフセット(y座標)
		unsigned short xAdvance;	//!	描画後のカーソル位置をずらすための値
		unsigned char page;			//! 文字テクスチャの番号(Pagesブロックの情報のインデックス)
		unsigned char chnl;			//! 文字データがどのチャネルの情報を持っているか(0000argbの各ビットに情報をもつ)
	};

	/**
	*	@struct Chars
	*	@brief	BMFontのCharsブロックに記述される情報
	*/
	struct Chars
	{
		std::vector<CharData> charDatas;
	};

	/**
	*	@struct KerningData
	*	@brief	BMFontのKerningPairブロックに記述されるカーニング1つあたりの情報
	*
	*	カーニング情報は、特定の文字と文字の間隔を調整するための情報
	*	一部の文字はほかの文字より近づけて配置する等の工夫が必要で、そのために使用する
	*/
	struct KerningData
	{
		unsigned int first;		//! 最初の文字情報
		unsigned int second;	//! 次の文字情報
		unsigned short amount;	//! 調整量(xAdvanceに加える) 
	};

	/**
	*	@struct KerningPair
	*	@brief	BMFontのKerningPairブロックに記述される情報
	*/
	struct KerningPair
	{
		std::vector<KerningData> kerningDatas;
	};

	/**
	*	@struct FontData
	*	@brief	BMFontのフォント情報
	*/
	struct FontData
	{
		Info info;
		Common common;
		Pages pages;
		Chars chars;
		KerningPair kerningPair;
	};

	/**
	*	@brief	BMFontデータを読み込む
	*	@param[in]	filePath : フォントデータのパス
	*	@param[out]	fontData : フォントデータ
	*	@retval		読み込み成功 : true
	*	@retval		読み込み失敗 : false
	*/
	bool LoadBMFont(const std::string& filePath, FontData& fontData);
}

