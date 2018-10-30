/*
*	@file Animation.h
*	@brief アニメーション管理クラス
*	@author Ishibashi Ryuto
*	@date
*	2018/10/01	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <map>
#include <memory>
#include <vector>

// 自作ヘッダインクルード
#include "../Math/Math.h"

// クラス使用宣言
class Bone;
class Pose;

struct KeyFrameData
{
	std::shared_ptr<Bone> bone;
	Math::Vector2 bazieCtrlPoint1;
	Math::Vector2 bazieCtrlPoint2;
};

class Animation
{
public:
	Animation();
	~Animation();

	/// @brief アニメーション情報の生成
	/// @note Animationクラスのインスタンスはこのメソッドを通じてのみ生成
	/// @param[in] frameCount : アニメーションの総フレーム数
	/// @retval	生成成功 : インスタンスのスマートポインタ
	/// @retval 生成失敗 : nullptr
	static std::shared_ptr<Animation> Create();

	/// @brief		アニメーション情報を追加する
	/// @param[in]	boneName		: ボーン名
	/// @param[in]	frame			: キーフレーム
	/// @param[in]	keyFrameData	: キーフレームのアニメーションデータ
	void AddKeyFrameData(const std::wstring& boneName, int frame, const KeyFrameData& keyFrameData);

	/// @brief 指定したフレームのモーションをモデルの姿勢情報に格納する
	/// @param[in]	frame	: モーションフレーム
	/// @param[out] pose	: 姿勢情報
	/// @note 呼出し時、姿勢計算まで行う
	void SetPose(int frame, std::shared_ptr<Pose> pose);

	///	@brief	アニメーションの時間を取得する
	int GetDuration() const;

private:
	int mDuration;														//! フレームカウント
	std::map<std::wstring, std::map< int, KeyFrameData> > mAnimationData;	//! アニメーションデータ

	float GetBezierCurveYWithNewton(float x, const Math::Vector2& ctrlPoint1, const Math::Vector2& ctrlPoint2, int n = 16);

	float GetBezierCurveYWithBisection(float x, const Math::Vector2& ctrlPoint1, const Math::Vector2& ctrlPoint2, int n = 8);
};

