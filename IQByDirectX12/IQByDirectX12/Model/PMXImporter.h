#pragma once
#include <cstdio>
#include <string>
#include "../Math/Math.h"

typedef struct
{
	char headerType[4];
	float version;
	unsigned char byteSize;
	unsigned char encodeType;
	unsigned char appendUvCount;
	unsigned char vertexIndexSize;
	unsigned char textureIndexSize;
	unsigned char materialIndexSize;
	unsigned char boneIndexSize;
	unsigned char morphIndexSize;
	unsigned char rigidIndexSize;
}PMXHeader;

typedef struct
{
	std::string modelName;
	std::string modelNameEng;
	std::string comment;
	std::string commentEng;
}ModelInfo;

typedef struct
{
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector2 uv;
}Vertex;

class PMXImporter
{
public:
	PMXImporter();
	~PMXImporter();

private:
};

