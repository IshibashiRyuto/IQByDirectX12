#include "Animation.h"

#include "Bone.h"
#include "Pose.h"

Animation::Animation()
	: mFrameCount(-1)
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

	if (mFrameCount < frame)
	{
		mFrameCount = frame;
	}
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
		auto postKeyFrameData = animationData.second.upper_bound(frame);
		auto preKeyFrameData = postKeyFrameData;
		preKeyFrameData--;
		if (postKeyFrameData == animationData.second.end())
		{
			postKeyFrameData = preKeyFrameData;
		}

		float t = static_cast<float>((*postKeyFrameData).first - (*preKeyFrameData).first);
		if (!Math::IsZero(t))
		{
			t = static_cast<float>(frame - (*preKeyFrameData).first) / t;
		}

		auto boneRot = Math::Slerp((*preKeyFrameData).second.bone->GetRotation(), (*postKeyFrameData).second.bone->GetRotation(), t);
		boneData[boneIndex]->SetRotation(boneRot);
	}
	pose->CalcPose();
}

float Animation::GetBezierCurveY(float x, const Math::Vector2& ctrlPoint1, const Math::Vector2& ctrlPoint2)
{
	if (Math::IsEqual(ctrlPoint1.x, ctrlPoint1.y)
		&& Math::IsEqual(ctrlPoint2.x, ctrlPoint2.y))
	{
		return x;
	}


	return 0.0f;
}
