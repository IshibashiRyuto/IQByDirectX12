#include "Animation.h"

#include "Bone.h"
#include "Pose.h"

Animation::Animation()
{
}


Animation::~Animation()
{
}

std::shared_ptr<Animation> Animation::Create()
{
	return std::shared_ptr<Animation>(new Animation());
}

void Animation::AddKeyFrameData(const std::wstring & boneName, int frame, const KeyFrameData & keyFrameData)
{
	mAnimationData[boneName][frame] = keyFrameData;
}

void Animation::SetPose(int frame, std::shared_ptr<Pose> pose)
{
	auto boneData = pose->GetBones();
	for (auto& animationData : mAnimationData)
	{
		if (!pose->IsFindBoneName(animationData.first))
		{
			continue;
		}
		
		auto boneIndex = pose->GetBoneIndex(animationData.first);
		auto preKeyFrameData = animationData.second.lower_bound(frame);
		auto postKeyFrameData = animationData.second.upper_bound(frame);
		float t = static_cast<float>(frame - (*preKeyFrameData).first) / static_cast<float>((*postKeyFrameData).first - (*preKeyFrameData).first);

		auto boneRot = Math::Slerp((*preKeyFrameData).second.bone->GetRotation(), (*postKeyFrameData).second.bone->GetRotation(), t);
		boneData[boneIndex]->SetRotation(boneRot);
	}
	pose->CalcPose();
}
