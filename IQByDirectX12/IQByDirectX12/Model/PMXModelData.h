/*
	@file		PMXModelData.h
	@brief		PMXモデルのデータを格納する
	@param		History
	2018/08/28	初版作成
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

