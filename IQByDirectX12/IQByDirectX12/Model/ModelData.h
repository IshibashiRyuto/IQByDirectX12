/*
	@file	ModelData.h
	@brief	���f���f�[�^���i�[����N���X
	@param	history
	2018/08/28	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include<memory>

//����w�b�_�C���N���[�h
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"

class ModelData
{
public:
	/// �R���X�g���N�^
	ModelData(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);

	/// �f�X�g���N�^
	~ModelData();

	/// ���_�o�b�t�@���擾����
	std::shared_ptr<VertexBuffer> GetVertexBuffer();

	/// �C���f�b�N�X�o�b�t�@���擾����
	std::shared_ptr<IndexBuffer> GetIndexBuffer();
	

protected:
	std::shared_ptr<VertexBuffer> mVertexBuffer;
	std::shared_ptr<IndexBuffer> mIndexBuffer;
private:
};

