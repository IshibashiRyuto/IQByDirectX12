#include "Pose.h"
#include "Bone.h"



Pose::Pose()
{
}

Pose::Pose(int boneCount)
{
	mBones.resize(boneCount);
	mChildBoneList = std::make_shared<ChildBoneList>();
	mBoneNameMap = std::make_shared<BoneNameMap>();
}

Pose::Pose(const Pose & src)
{
	this->mBones = std::vector<std::shared_ptr<Bone>>(src.mBones.begin(), src.mBones.end());
	this->mBoneNameMap = src.mBoneNameMap;
	this->mChildBoneList = src.mChildBoneList;
}

Pose::~Pose()
{
}

std::shared_ptr<Pose> Pose::Create(int boneCount)
{
	return std::shared_ptr<Pose>(new Pose(boneCount));
}

std::shared_ptr<Pose> Pose::Create(std::shared_ptr<Pose> defaultPose)
{
	return std::shared_ptr<Pose>(new Pose(*defaultPose));
}

void Pose::CalcPose()
{
	if (!mChildBoneList)
	{
		return;
	}
	CalcBoneMatrix(Math::CreateIdent(), (*mChildBoneList)[ROOT_BONE_INDEX] );
}

const std::vector<std::shared_ptr<Bone>>& Pose::GetBones()
{
	return mBones;
}

bool Pose::IsFindBoneName(const std::wstring & boneName)
{
	return mBoneNameMap->find(boneName) != mBoneNameMap->end();
}

int Pose::GetBoneIndex(const std::wstring & boneName)
{
	return (*mBoneNameMap)[boneName];
}

void Pose::SetBoneData(const std::wstring& boneName, const std::shared_ptr<Bone> bone, int boneIndex, int parentBoneIndex)
{
	if (boneIndex < 0 
		|| static_cast<unsigned int>(boneIndex) >= mBones.size() 
		|| !mChildBoneList)
	{
		return;
	}
	mBones[boneIndex] = bone;
	(*mChildBoneList)[parentBoneIndex].push_back(boneIndex);
	(*mBoneNameMap)[boneName] = boneIndex;
}

std::shared_ptr<Pose> Pose::Lerp(const std::shared_ptr<Pose> prePose, const std::shared_ptr<Pose> postPose, float time)

{
	// 同一のボーン構造を参照していない場合、空の姿勢情報を返す
	if (prePose->mChildBoneList != postPose->mChildBoneList)
	{
		return nullptr;
	}

	time = Math::Clamp(time, 0.0f, 1.0f);
	auto lerpPose = Create(prePose);

	for (unsigned int i = 0; i < lerpPose->mBones.size(); ++i)
	{
		auto preRot = prePose->mBones[i]->GetRotation();
		auto postRot = postPose->mBones[i]->GetRotation();
		lerpPose->mBones[i]->SetRotation(Math::Slerp(preRot,postRot,time));
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
		childBone->Rotate(mat);
		auto rotMat = childBone->GetBoneMatrix();


		CalcBoneMatrix(rotMat, (*mChildBoneList)[childBoneIndex]);
	}
}
