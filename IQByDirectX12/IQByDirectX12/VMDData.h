/*
*	@file VMDData.h
*	@brief VMDモーションデータを扱うクラスを記述する
*	@author Ishibashi Ryuto
*	@date
*	2018/09/25	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <vector>
#include <map>
#include <string>

// 自作ヘッダインクルード
#include "Math\Math.h"

namespace VMD
{
	struct Header
	{
		char header[30];
		char modelName[20];
	};

	struct MotionDataInfo
	{
		char boneName[15];					// ボーン名
		unsigned int frameNo;				// フレーム番号
		Math::Vector3 location;				// 位置
		Math::Quaternion rotation;			// 回転
		unsigned char interporation[64];	// 補間データ
	};
	

}


struct MotionData
{
	std::string boneName;

};

class VMDData
{
public:
	VMDData();
	~VMDData();
	
	std::map<int, std::vector<MotionData>> animationData;
};

