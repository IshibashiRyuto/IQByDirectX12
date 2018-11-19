#include "BMFontData.h"
#include <cstdio>
#include "../Debug/DebugLayer.h"
#include "../ConvertString.h"

using namespace BMFont;

const int TEXT_RANGE_MAX = 1024;	//! �ǂݍ��݃e�L�X�g�����̌��E

std::string _ReadTextEndNull(FILE* fp);

bool _LoadInfoBlock(FILE* fp, Info& info);

bool _LoadCommonBlock(FILE* fp, Common& common);

bool _LoadPagesBlock(FILE* fp, Pages& pages, int pageNum);

bool _LoadCharsBlock(FILE* fp, Chars& chars);

bool _LoadKerningPairBlock(FILE* fp, KerningPair& kerningPair);

bool BMFont::LoadBMFont(const std::string & filePath, FontData & fontData)
{
	FILE *fp;
	auto err = fopen_s(&fp, filePath.c_str(), "rb");
	if (err != 0)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Font File [" + filePath + "] is Not Found.");
		return false;
	}

	char signature[4];
	fread(signature, sizeof(char), 4, fp);

	if ((signature[0] != 'B') ||
		(signature[1] != 'M') ||
		(signature[2] != 'F'))
	{
		DebugLayer::GetInstance().PrintDebugMessage("This file is not font file.");
		fclose(fp);
		return false;
	}

	if ((signature[3] != 3))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Program not support version.");
		DebugLayer::GetInstance().PrintDebugMessage("Please use newest version.");
		fclose(fp);
		return false;
	}

	if (!_LoadInfoBlock(fp, fontData.info) ||
		!_LoadCommonBlock(fp, fontData.common) ||
		!_LoadPagesBlock(fp, fontData.pages, fontData.common.pages) ||
		!_LoadCharsBlock(fp, fontData.chars) ||
		!_LoadKerningPairBlock(fp, fontData.kerningPair))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Font file data error.");
		return false;
	}

	return true;
}

std::string _ReadTextEndNull(FILE * fp)
{
	char text[TEXT_RANGE_MAX];
	
	for (int i = 0; i < TEXT_RANGE_MAX; ++i)
	{
		fread(&text[i], sizeof(char), 1, fp);
		if (text[i] == '\0')
		{
			break;
		}
	}

	return std::string(text);
}

bool _LoadInfoBlock(FILE * fp, Info & info)
{
	// Info�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 4L, SEEK_SET);

	// Info�u���b�N�ǂݍ��ݏ���
	BlockType blockType;
	char tmp;
	fread(&tmp, sizeof(char), 1, fp);
	blockType = static_cast<BlockType>(tmp);
	if (blockType != BlockType::Info)
	{
		return false;
	}
	
	int blockSize;
	fread(&blockSize, sizeof(blockSize), 1, fp);
	
	// �ϒ����ȊO�ǂݍ���(14�o�C�g�Œ�)
	fread(&info, 14, 1, fp);

	// �t�H���g���ǂݍ���(�ϒ�)
	info.fontName = _ReadTextEndNull(fp);
	info.wstringFontName = ConvertStringToWString(info.fontName);

	return true;
}

bool _LoadCommonBlock(FILE * fp, Common & common)
{
	// Info�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 4, SEEK_SET);

	// Common�u���b�N�J�n�n�_�܂ňړ�
	int seekSize;
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// Common�u���b�N�ǂݍ��ݏ���
	BlockType blockType;
	char tmp;
	fread(&tmp, sizeof(char), 1, fp);
	blockType = static_cast<BlockType>(tmp);
	if (blockType != BlockType::Common)
	{
		return false;
	}

	int blockSize;
	fread(&blockSize, sizeof(blockSize), 1, fp);

	fread(&common, sizeof(Common), 1, fp);

	return true;
}

bool _LoadPagesBlock(FILE * fp, Pages & pages, int pageNum)
{
	// Info�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 4, SEEK_SET);

	// Common�u���b�N�J�n�n�_�܂ňړ�
	int seekSize;
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// Pages�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// Pages�u���b�N�ǂݍ��ݏ���
	BlockType blockType;
	char tmp;
	fread(&tmp, sizeof(char), 1, fp);
	blockType = static_cast<BlockType>(tmp);
	if (blockType != BlockType::Pages)
	{
		return false;
	}

	int blockSize;
	fread(&blockSize, sizeof(blockSize), 1, fp);

	pages.pageNames.resize(pageNum);
	pages.wstringPageNames.resize(pageNum);

	for (int i = 0; i < pageNum; ++i)
	{
		pages.pageNames[i] = _ReadTextEndNull(fp);
		pages.wstringPageNames[i] = ConvertStringToWString(pages.pageNames[i]);
	}
	return true;
}

bool _LoadCharsBlock(FILE * fp, Chars & chars)
{
	// Info�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 4, SEEK_SET);

	// Common�u���b�N�J�n�n�_�܂ňړ�
	int seekSize;
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// Pages�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// Chars�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// Chars�u���b�N�ǂݍ��ݏ���
	BlockType blockType;
	char tmp;
	fread(&tmp, sizeof(char), 1, fp);
	blockType = static_cast<BlockType>(tmp);
	if (blockType != BlockType::Chars)
	{
		return false;
	}

	int blockSize;
	fread(&blockSize, sizeof(blockSize), 1, fp);

	int charNum = blockSize / sizeof(CharData);

	chars.charDatas.resize(charNum);
	fread(chars.charDatas.data(), sizeof(CharData), charNum, fp);

	return true;
}

bool _LoadKerningPairBlock(FILE * fp, KerningPair & kerningPair)
{
	// Info�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 4, SEEK_SET);

	// Common�u���b�N�J�n�n�_�܂ňړ�
	int seekSize;
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// Pages�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// Chars�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// KerningPair�u���b�N�J�n�n�_�܂ňړ�
	fseek(fp, 1, SEEK_CUR);
	fread(&seekSize, sizeof(int), 1, fp);
	fseek(fp, seekSize, SEEK_CUR);

	// KerningPair�u���b�N�ǂݍ��ݏ���
	BlockType blockType;
	char tmp;
	fread(&tmp, sizeof(char), 1, fp);
	blockType = static_cast<BlockType>(tmp);
	if (blockType != BlockType::KerningPair)
	{
		return false;
	}

	int blockSize;
	fread(&blockSize, sizeof(blockSize), 1, fp);

	int kerningPairNum = blockSize / sizeof(KerningData);
	kerningPair.kerningDatas.resize(kerningPairNum);

	fread(kerningPair.kerningDatas.data(), sizeof(KerningData), kerningPairNum, fp);
	
	return true;
}
