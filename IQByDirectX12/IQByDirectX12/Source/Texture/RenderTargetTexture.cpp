#include "RenderTargetTexture.h"



RenderTargetTexture::RenderTargetTexture(ComPtr<ID3D12Resource> textureData)
	: Texture(textureData)
{
}


RenderTargetTexture::~RenderTargetTexture()
{
}
