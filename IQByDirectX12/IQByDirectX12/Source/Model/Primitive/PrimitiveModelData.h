/*
*	@file	PrimitiveModelData.h
*	@brief	プリミティブのモデルデータクラスを記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/19	初版作成
*/
#pragma once
#include "../ModelData.h"

class PrimitiveModelData :
	public ModelData
{
public:
	PrimitiveModelData(std::shared_ptr<VertexBuffer> vertexBuffer,
		std::shared_ptr<IndexBuffer> indexBuffer,
		std::shared_ptr<DescriptorHeap> descHeap,
		std::shared_ptr<PipelineStateObject> pipelineStateObject);
	~PrimitiveModelData();
};

