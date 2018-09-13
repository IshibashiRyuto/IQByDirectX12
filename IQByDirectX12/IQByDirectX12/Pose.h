/*
	Pose.h
	ボーン情報を統括し、姿勢情報を格納する
	@author Ishibashi Ryuto
	@date
	2018/09/12	初版作成
*/
#pragma once
/* システムヘッダインクルード */
#include <memory>
#include <list>
#include <vector>
#include <map>

/* 自作ヘッダインクルード */
#include "Math/Math.h"

/* クラス使用宣言 */
class Bone;

typedef std::map< int, std::list<int>> ChildBoneList;

class Pose
{
public:
	/// コンストラクタ
	Pose();

	/// コンストラクタ
	/// @param[in] boneCount : 姿勢のボーン数
	Pose(int boneCount);

	/// コピーコンストラクタ
	Pose(const Pose& src);

	/// デストラクタ
	~Pose();

	/// @fn CalcPose
	/// 姿勢計算を行う
	void CalcPose();

	/// @fn GetBones
	/// ボーン配列を取得する
	/// @retval ボーン配列の参照
	const std::vector<Bone> & GetBones();

	/// @fn SetBoneData
	/// ボーン情報を追加する
	/// @param[in] bone	:	 ボーン情報
	/// @param[in] boneIndex : 自身のインデックス
	/// @param[in] parentBoneIndex : 親のインデックス
	/// @note すべての親ノードは-1とし、親ノードに接続されていないボーンは
	/// 姿勢計算対象外になる
	void SetBoneData(const Bone& bone, int boneIndex, int parentBoneIndex = -1);

	/// @fn Lerp
	/// 姿勢情報を補間する
	/// @param[in] prePose : time = 0.0 の場合の姿勢
	/// @param[in] postPose : time = 1.0 の場合の姿勢
	/// @param[in] time	: 補間係数( 0.0 <= time <= 1.0 にクランプされる )
	/// @retval 姿勢行列計算前の姿勢
	static Pose Lerp(const Pose& prePose, const Pose& postPose, float time);

private:
	/* 定数定義 */
	const int ROOT_BONE_INDEX = -1;

	/* メンバ変数宣言 */
	std::vector<Bone> mBones;
	std::shared_ptr< ChildBoneList > mChildBoneList;
	
	/* プライベートメソッド定義 */
	
	/// @fn CalcBoneMatrix
	/// ボーン行列を計算する
	/// @param[in] matrix : ボーン変形行列
	/// @param[in] childList : 自身の子ボーン
	void CalcBoneMatrix(const Math::Matrix4x4& mat, const std::list<int> & childBoneList);
};

