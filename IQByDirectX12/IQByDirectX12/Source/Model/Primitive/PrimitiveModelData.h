/*
*	@file	PrimitiveModelData.h
*	@brief	�v���~�e�B�u�̃��f���f�[�^�N���X���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/19	���ō쐬
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

