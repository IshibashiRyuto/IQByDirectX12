#include "Animation.h"

#include "Bone.h"
#include "Pose.h"

Animation::Animation()
	: mDuration(-1)
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

	if (mDuration < frame)
	{
		mDuration = frame;
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
			t = GetBezierCurveYWithBisection(t, (*postKeyFrameData).second.bazieCtrlPoint1, (*postKeyFrameData).second.bazieCtrlPoint2, 16);
		}
		if (frame == 11 && boneIndex == 72)
		{
			int a = 0;
		}

		auto boneRot = Math::Slerp((*preKeyFrameData).second.bone->GetRotation(), (*postKeyFrameData).second.bone->GetRotation(), t);
		
		boneData[boneIndex]->SetRotation(boneRot);
	}
	pose->CalcPose();
}

int Animation::GetDuration() const
{
	return mDuration;
}

float Animation::GetBezierCurveYWithNewton(float x, const Math::Vector2& ctrlPoint1, const Math::Vector2& ctrlPoint2, int n)
{
	if (Math::IsEqual(ctrlPoint1.x, ctrlPoint1.y)
		&& Math::IsEqual(ctrlPoint2.x, ctrlPoint2.y))
	{
		return x;
	}

	float t = x;
	float k0 = (1 + 3 * ctrlPoint1.x - 3 * ctrlPoint2.x);
	float k1 = (3 * ctrlPoint2.x - 6 * ctrlPoint1.x);
	float k2 = 3 * ctrlPoint2.x;

	for (int i = 0; i < n; ++i)
	{
		//t2 = t1 - (f(x)) / (f'(x))
		float f = t * t * t * k0 + t * t*k1 + t * k2 - x;
		float df = 3 * t * t * k0 + 2 * t*k1 + k2;
		if (Math::IsZero(df) || Math::IsZero(f))
		{
			break;
		}

		t = t - f / df;
	}

	t = Math::Clamp(t, 0.0f, 1.0f);
	float r = 1 - t;

	return 3*r*r*t*ctrlPoint1.y + 3 * r*t*t * ctrlPoint2.y + t*t*t;
}

float Animation::GetBezierCurveYWithBisection(float x, const Math::Vector2 & ctrlPoint1, const Math::Vector2 & ctrlPoint2, int n)
{
	if (Math::IsEqual(ctrlPoint1.x, ctrlPoint1.y)
		&& Math::IsEqual(ctrlPoint2.x, ctrlPoint2.y))
	{
		return x;
	}

	float t = 0.0f;
	float top = 1.0f;
	float bottom = 0.0f;

	for (int i = 0; i < n; ++i)
	{
		t = (top + bottom) / 2.f;
		float r = 1 - t;
		float f = 3 * r*r*t*ctrlPoint1.x + 3 * r*t*t*ctrlPoint2.x + t * t*t - x;
		
		if (Math::IsZero(f))
		{
			break;
		}

		if (f < 0.f)
		{
			bottom = t;
		}
		else
		{
			top = t;
		}
	}

	t = Math::Clamp(t, 0.0f, 1.0f);
	float r = 1 - t;

	return 3 * r*r*t*ctrlPoint1.y + 3 * r*t*t * ctrlPoint2.y + t * t*t;
}
