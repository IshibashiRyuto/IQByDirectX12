#pragma once
#include "Texture.h"
class RenderTargetTexture :
	public Texture
{
public:
	RenderTargetTexture(ComPtr<ID3D12Resource> textureData);
	~RenderTargetTexture();
};

