/*
	Bone.h
	ボーン情報を格納するクラス
	@author Ishibashi Ryuto
	@date 
	2018/09/11	初版作成
	2018/09/14	コンストラクタを隠蔽、shared_ptrを返すCreate関数を作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>

// 自作ヘッダインクルード
#include "Math/Math.h"

class Bone
{
public:

	/// デストラクタ
	~Bone();

	/// ボーン情報を生成する
	static std::shared_ptr<Bone> Create(const Math::Vector3 & headPosition);

	/// ボーンの回転を設定する
	void SetRotation(const Math::Quaternion& rotation);
	void SetRotation(const Math::Vector3& rotation);
	void SetRotation(float rotX, float rotY, float rotZ);

	/// 軸制限の使用設定を行う
	void SetLimitAxis(bool isUse, const Math::Vector3& axis);

	/// ボーンを回転させる
	void Rotate(const Math::Matrix4x4& matrix);

	/// ボーンの回転情報を取得する
	const Math::Quaternion& GetRotation() const;

	/// ボーン回転行列を取得する
	const Math::Matrix4x4 GetBoneMatrix() const;
private:
	/* メンバ変数定義 */
	Math::Vector3 mOriginHeadPosition;		// ボーン初期ヘッド位置
	Math::Vector3 mRotatedHeadPosition;		// 回転操作後のボーンヘッド位置
	Math::Quaternion mRotation;				// ボーンの回転量
	Math::Quaternion mRotatedRotation;		// 回転操作後のボーン回転量
	Math::Matrix4x4 mMatrix;
	Math::Vector3	mLimitAxis;				// 制限軸
	bool			mIsUseLimitAxis;		// 軸制限機能を使用するか
	/* プライベートメソッド定義 */
	/// コンストラクタ
	Bone(const Math::Vector3 & headPosition);
};

