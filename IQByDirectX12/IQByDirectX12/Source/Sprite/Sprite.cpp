#include "Sprite.h"
#include "../InstancingDataManager.h"
#include "../Texture/Texture.h"
#include "SpriteVertex.h"
#include "SpriteData.h"
#include "SpriteDataManager.h"

using namespace Math;

std::shared_ptr<PipelineStateObject> Sprite::sPSO;

Sprite::Sprite(int spriteHandle)
	: mSpriteHandle(spriteHandle)
	, mInstDataMgr(InstancingDataManager::GetInstance())
	, mPos(0.0f, 0.0f)
	, mScale(1.0f, 1.0f)
	, mAngle(0.0f)
{
	CalcMatrix();
}


Sprite::~Sprite()
{
}

void Sprite::SetPipelineStateObject(std::shared_ptr<PipelineStateObject> pso)
{
	sPSO = pso;
}

std::shared_ptr<Sprite> Sprite::Create(std::shared_ptr<Device> device, std::shared_ptr<Texture> texture)
{
	auto resDesc = texture->GetTextureData()->GetDesc();
	
	// 頂点情報作成
	// 頂点はTriangleStrip
	// 左上→右上→左下→右下の順に作成
	// 座標系はwindow座標系(右下正の平面)
	SpriteVertex vert[4];
	float width = resDesc.Width;
	float height = resDesc.Height;
	vert[0].pos = Vector4(-1, 1, 0.0f, 0.0f);
	vert[0].uv = Vector2(0.0f, 0.0f);
	vert[1].pos = Vector4(1, 1, 0.0f, 0.0f);
	vert[1].uv = Vector2(1.0f, 0.0f);
	vert[2].pos = Vector4(-1, -1, 0.0f, 0.0f);
	vert[2].uv = Vector2(0.0f, 1.0f);
	vert[3].pos = Vector4(1, -1, 0.0f, 0.0f);
	vert[3].uv = Vector2(1.0f, 1.0f);

	auto vertBuf = VertexBuffer::Create(device, vert, 4, sizeof(SpriteVertex));
	auto descHeap = DescriptorHeap::Create(device, 1);
	if (!descHeap)
	{
		return nullptr;
	}
	descHeap->SetTexture(texture, 0);
	

	auto spriteData = SpriteData::Create(vertBuf, descHeap, sPSO);
	if (!spriteData)
	{
		return nullptr;
	}

	int spriteHandle = SpriteDataManager::GetInstance().Regist(spriteData);

	return std::shared_ptr<Sprite>(new Sprite(spriteHandle));
}

void Sprite::Draw() const
{
	mInstDataMgr.SetInstanceData(mSpriteHandle, (void*)&mInstMat, sizeof(mInstMat));
}

void Sprite::SetPosition(float x, float y)
{
	mPos.x = x;
	mPos.y = y;
	CalcMatrix();
}

void Sprite::SetPosition(const Math::Vector2& pos)
{
	mPos = pos;
	CalcMatrix();
}

void Sprite::SetRotation(float rad)
{
	mAngle = rad;
	CalcMatrix();
}

void Sprite::SetScale(float scale)
{
	mScale.x = scale;
	mScale.y = scale;
	CalcMatrix();
}

void Sprite::SetScale(const Math::Vector2& scale)
{
	mScale = scale;
	CalcMatrix();
}

void Sprite::SetTexture(std::shared_ptr<Texture> texture)
{
	mTexture = texture;
}

std::shared_ptr<Sprite> Sprite::Duplicate()
{
	return std::shared_ptr<Sprite>(new Sprite(mSpriteHandle));
}

void Sprite::CalcMatrix()
{
	mInstMat = CreateIdentMat3x3();
	mInstMat *= CreateScaleMatrix3x3(mScale);
	mInstMat *= CreateRotMatrix3x3(mAngle);
	mInstMat *= CreateTranslateMatrix3x3(mPos);
}
