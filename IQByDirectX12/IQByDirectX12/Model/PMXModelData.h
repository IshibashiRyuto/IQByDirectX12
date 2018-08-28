/*
	@file		PMXModelData.h
	@brief		PMX���f���̃f�[�^���i�[����
	@param		History
	2018/08/28	���ō쐬
*/
#pragma once
#include<memory>

class VertexBuffer;
class IndexBuffer;

class PMXModelData
{
public:
	PMXModelData();
	~PMXModelData();
private:
	std::shared_ptr<VertexBuffer>	mVertexBuffer;
	std::shared_ptr<IndexBuffer>	mIndexBuffer;
};

