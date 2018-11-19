/**
*	@file	BitMapFontData.h
*	@brief	�t�H���g�f�[�^���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/19	���ō쐬
*
*	BMFont�ō쐬�����t�H���g�f�[�^�̃f�[�^�\��
*	�t�H���g�f�[�^��5�̃u���b�N���琬��
*	�Q�l:http://www.angelcode.com/products/bmfont/doc/file_format.html
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <string>
#include <vector>

/**
*	@namespace	BMFont
*	@brief		BMFont�֘A�̃f�[�^����у��\�b�h���L�q����
*/

namespace BMFont
{

	/**
	*	@enum	BlockType
	*	@brief	�r�b�g�}�b�v�t�H���g�̃u���b�N�^�C�v
	*/
	enum class BlockType
	{
		Info = 1,
		Common,
		Pages,
		Chars,
		KerningPair
	};

	/**
	*	@struct Info
	*	@brief	BMFont��Info�u���b�N�ɋL�q�������
	*/
	struct Info
	{
		short fontSize;	//! �t�H���g�T�C�Y
		union
		{
			struct
			{
				bool reserved : 3;		//!	�\��̈�
				bool fixedHeight : 1;	//!	�����C�������邩
				bool bold : 1;			//! �{�[���h�̂�
				bool italic : 1;		//! �C�^���b�N�̂�
				bool unicode : 1;		//! Unicode���g�p���Ă��邩
				bool smooth : 1;		//! �X���[�X�t�H���g��
			};
			char bitField;	//!	�r�b�g�t�B�[���h
		};
		unsigned char charSet;			//! �����Z�b�g
		unsigned short stretchH;			//! �����X�g���b�`
		unsigned char aa;				//! �A���`�G�C���A�X
		unsigned char paddingUp;		//! ������̃p�f�B���O����(��)
		unsigned char paddingRight;		//! ������̃p�f�B���O����(�E)
		unsigned char paddingDown;		//! ������̃p�f�B���O����(��)
		unsigned char paddingLeft;		//! ������̃p�f�B���O����(��)
		unsigned char spacingHoriz;		//! ���������̕����Ԋu
		unsigned char spacingVert;		//! ���������̕����Ԋu
		unsigned char outline;			//! �A�E�g���C��
		std::string fontName;			//! �t�H���g��
		std::wstring wstringFontName;	//! �t�H���g��(wstring��)
	};

	/**
	*	@struct	Common
	*	@brief	BMFont��Common�u���b�N�ɋL�q�������
	*/
	struct Common
	{
		unsigned short lineHeight;		//! �e�L�X�g1�s������̍���(pixel)
		unsigned short base;			//! �s�̏�[����x�[�X���C���܂ł̋���(pixel)
		unsigned short scaleW;			//! �t�H���g�e�N�X�`��1��������̉���(pixel)
		unsigned short scaleH;			//! �t�H���g�e�N�X�`��1��������̏c��(pixel)
		unsigned short pages;			//! �y�[�W�̐�
		union
		{
			char bitField;				//! �r�b�g�t�B�[���h
			struct
			{
				bool packed : 1;		//! �p�b�N����Ă��邩
				bool reserved : 7;		//! �\��̈�
			};
		};
		unsigned char alphaChnl;		//! �A���t�@�`���l���̏��
		unsigned char redChnl;			//! �ԃ`���l���̏��
		unsigned char greenChnl;		//! �΃`���l���̏��
		unsigned char blueChnl;			//! �`���l���̏��
	};

	/**
	*	@struct Pages
	*	@brief	BMFont��Pages�u���b�N�ɋL�q�������
	*/
	struct Pages
	{
		std::vector<std::string> pageNames;			//! �y�[�W�����X�g
		std::vector<std::wstring> wstringPageNames;	//! �y�[�W�����X�g(wstinrg��)	
	};

	/**
	*	@struct CharData
	*	@brief	BMFont��Chars�u���b�N�ɋL�q�����A1����������̏��
	*/
	struct CharData
	{
		unsigned int id;			//! �����R�[�h
		unsigned short x;			//! �摜�̐؂���ʒu(x���W)
		unsigned short y;			//! �摜�̐؂���ʒu(y���W)
		unsigned short width;		//! �摜�̐؂��蕝
		unsigned short height;		//! �摜�̐؂��荂��
		unsigned short xOffset;		//! �`��I�t�Z�b�g(x���W)
		unsigned short yOffset;		//! �`��I�t�Z�b�g(y���W)
		unsigned short xAdvance;	//!	�`���̃J�[�\���ʒu�����炷���߂̒l
		unsigned char page;			//! �����e�N�X�`���̔ԍ�(Pages�u���b�N�̏��̃C���f�b�N�X)
		unsigned char chnl;			//! �����f�[�^���ǂ̃`���l���̏��������Ă��邩(0000argb�̊e�r�b�g�ɏ�������)
	};

	/**
	*	@struct Chars
	*	@brief	BMFont��Chars�u���b�N�ɋL�q�������
	*/
	struct Chars
	{
		std::vector<CharData> charDatas;
	};

	/**
	*	@struct KerningData
	*	@brief	BMFont��KerningPair�u���b�N�ɋL�q�����J�[�j���O1������̏��
	*
	*	�J�[�j���O���́A����̕����ƕ����̊Ԋu�𒲐����邽�߂̏��
	*	�ꕔ�̕����͂ق��̕������߂Â��Ĕz�u���铙�̍H�v���K�v�ŁA���̂��߂Ɏg�p����
	*/
	struct KerningData
	{
		unsigned int first;		//! �ŏ��̕������
		unsigned int second;	//! ���̕������
		unsigned short amount;	//! ������(xAdvance�ɉ�����) 
	};

	/**
	*	@struct KerningPair
	*	@brief	BMFont��KerningPair�u���b�N�ɋL�q�������
	*/
	struct KerningPair
	{
		std::vector<KerningData> kerningDatas;
	};

	/**
	*	@struct FontData
	*	@brief	BMFont�̃t�H���g���
	*/
	struct FontData
	{
		Info info;
		Common common;
		Pages pages;
		Chars chars;
		KerningPair kerningPair;
	};

	/**
	*	@brief	BMFont�f�[�^��ǂݍ���
	*	@param[in]	filePath : �t�H���g�f�[�^�̃p�X
	*	@param[out]	fontData : �t�H���g�f�[�^
	*	@retval		�ǂݍ��ݐ��� : true
	*	@retval		�ǂݍ��ݎ��s : false
	*/
	bool LoadBMFont(const std::string& filePath, FontData& fontData);
}

