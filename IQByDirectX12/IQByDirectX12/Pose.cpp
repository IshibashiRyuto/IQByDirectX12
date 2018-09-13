#include "Pose.h"
#include "Bone.h"



Pose::Pose()
{
}

Pose::Pose(int boneCount)
{
	mBones.resize(boneCount);
	mChildBoneList = std::make_shared<ChildBoneList>();
}

Pose::Pose(const Pose & src)
{
	this->mBones = std::vector<Bone>(src.mBones.begin(), src.mBones.end());
	this->mChildBoneList = src.mChildBoneList;
}

Pose::~Pose()
{
}

void Pose::CalcPose()
{
	if (!mChildBoneList)
	{
		return;
	}
	CalcBoneMatrix(Math::CreateIdent(), (*mChildBoneList)[ROOT_BONE_INDEX] );
}

const std::vector<Bone>& Pose::GetBones()
{
	return mBones;
}

void Pose::SetBoneData(const Bone & bone, int boneIndex, int parentBoneIndex)
{
	if (boneIndex < 0 
		|| static_cast<unsigned int>(boneIndex) >= mBones.size() 
		|| !mChildBoneList)
	{
		return;
	}
	mBones[boneIndex] = bone;
	(*mChildBoneList)[parentBoneIndex].push_back(boneIndex);
}

Pose Pose::Lerp(const Pose & prePose, const Pose & postPose, float time)
{
	if (prePose.mChildBoneList != postPose.mChildBoneList)
	{
		return Pose();
	}
	time = Math::Clamp(time, 0.0f, 1.0f);
	Pose lerpPose(prePose);
	for (unsigned int i = 0; i < lerpPose.mBones.size(); ++i)
	{
		auto preRot = prePose.mBones[i].GetRotation();
		auto postRot = postPose.mBones[i].GetRotation();
		lerpPose.mBones[i].SetRotation(Math::Slerp(preRot,postRot,time));
	}
	return lerpPose;
}

void Pose::CalcBoneMatrix(const Math::Matrix4x4 & mat, const std::list<int>& childBoneList)
{
	if (!mChildBoneList)
	{
		return;
	}

	for (auto childBoneIndex : childBoneList)
	{
		if (childBoneIndex < 0 || static_cast<unsigned int>(childBoneIndex) >= mBones.size())
		{
			continue;
		}
		auto& childBone = mBones[childBoneIndex];
		childBone.Rotate(mat);
		auto rotMat = childBone.GetBoneMatrix() * mat;
		CalcBoneMatrix(rotMat, (*mChildBoneList)[childBoneIndex]);
	}
}
