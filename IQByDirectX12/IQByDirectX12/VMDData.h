/*
*	@file VMDData.h
*	@brief VMD���[�V�����f�[�^�������N���X���L�q����
*	@author Ishibashi Ryuto
*	@date
*	2018/09/25	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <vector>
#include <map>
#include <string>

// ����w�b�_�C���N���[�h
#include "Math\Math.h"

#pragma pack(1)

namespace VMD
{
	struct Header
	{
		char header[30];
		char modelName[20];
	};

	struct MotionDataInfo
	{
		char boneName[15];					// �{�[����
		unsigned int frameNo;				// �t���[���ԍ�
		Math::Vector3 location;				// �ʒu
		Math::Quaternion rotation;			// ��]
		unsigned char interpolation[64];	// ��ԃf�[�^
	};
	

}

#pragma pack()

struct MotionData
{
	std::string boneName;
	Math::Quaternion rotatation;
};

class VMDData
{
public:
	VMDData();
	~VMDData();
	
	std::map<int, std::vector<MotionData>> animationData;
};

