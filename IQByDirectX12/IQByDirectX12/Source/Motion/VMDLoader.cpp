#include <stdio.h>
#include <locale>
#include <codecvt>
#include <windows.h>
#include "VMDLoader.h"

#include "VMDData.h"
#include "Animation.h"
#include "Bone.h"
#include "../ConvertString.h"


VMDLoader::VMDLoader()
{
}


VMDLoader::~VMDLoader()
{
}

std::shared_ptr<VMDLoader> VMDLoader::Create()
{
	return std::shared_ptr<VMDLoader>(new VMDLoader());
}

std::shared_ptr<Animation> VMDLoader::Load(const std::string & filePath)
{
	auto animationData = Animation::Create();
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, filePath.c_str(), "r");
	if (err != 0)
	{
		return nullptr;
	}

	VMD::Header header;
	unsigned int frameDataCount;
	std::vector<VMD::MotionDataInfo> motionData;

	fread(&header, sizeof(header), 1, fp);
	fread(&frameDataCount, sizeof(frameDataCount), 1, fp);

	motionData.resize(frameDataCount);

	fread(motionData.data(), sizeof(VMD::MotionDataInfo), motionData.size(), fp);

	fclose(fp);

	for (auto& data : motionData)
	{
		KeyFrameData keyFrameData;
		keyFrameData.bone = Bone::Create(Math::Vector3());
		keyFrameData.bone->SetRotation(data.rotation);
		std::wstring boneName;

		std::string str(data.boneName);

		boneName = ConvertStringToWString(str);

		animationData->AddKeyFrameData(boneName, data.frameNo, keyFrameData);
	}

	return animationData;
}
