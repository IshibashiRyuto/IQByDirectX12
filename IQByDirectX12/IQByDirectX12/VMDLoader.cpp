#include <stdio.h>
#include "VMDLoader.h"

#include "VMDData.h"


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

void VMDLoader::Load(const std::string & filePath)
{
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, filePath.c_str(), "r");
	if (err != 0)
	{
		return;
	}

	VMD::Header header;
	unsigned int frameDataCount;
	std::vector<VMD::MotionDataInfo> motionData;

	fread(&header, sizeof(header), 1, fp);
	fread(&frameDataCount, sizeof(frameDataCount), 1, fp);

	motionData.resize(frameDataCount);

	fread(motionData.data(), sizeof(VMD::MotionDataInfo), motionData.size(), fp);

	fclose(fp);
}
