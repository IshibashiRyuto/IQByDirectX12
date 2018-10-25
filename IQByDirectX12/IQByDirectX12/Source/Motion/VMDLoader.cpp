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
	err = fopen_s(&fp, filePath.c_str(), "rb");
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

	auto size = sizeof(VMD::MotionDataInfo);

	for (int i = 0; i < frameDataCount; ++i)
	{
		int dataCnt = fread(&motionData[i], size, 1, fp);
		int err = ferror(fp);
		int end = feof(fp);
		int a = 0;
	}

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
