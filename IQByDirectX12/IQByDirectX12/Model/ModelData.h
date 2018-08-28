/*
	@file	ModelData.h
	@brief	モデルデータを格納するクラス
	@param	history
	2018/08/28	初版作成
*/
#pragma once
// システムヘッダインクルード
#include<memory>

//自作ヘッダインクルード
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"

class ModelData
{
public:
	ModelData();
	~ModelData();

protected:
	std::shared_ptr<VertexBuffer> mVertexBuffer;
	std::shared_ptr<IndexBuffer> mIndexBuffer;
private:
};

