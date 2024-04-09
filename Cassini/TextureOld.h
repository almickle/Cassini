#pragma once
#include "GraphicsResource.h"

class TextureOld : public GraphicsResource
{
public:
	TextureOld(Graphics& gfx, UINT resolution[2]) {

		D3D11_TEXTURE2D_DESC textureDesc;

		// Initialize the  texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the texture description.
		textureDesc.Width = resolution[0];
		textureDesc.Height = resolution[1];
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the texture
		gfx.GetDevice()->CreateTexture2D(&textureDesc, NULL, &pTexture);
	}

private:
	ComPtr <ID3D11Texture2D> pTexture;
};
