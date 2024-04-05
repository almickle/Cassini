#pragma once
#include "DxgiInfoManager.h"
#include "GraphicsResource.h"

class Texture3D : public GraphicsResource
{
public:
	Texture3D(Graphics& gfx, const UINT resolution[3]) {
		D3D11_TEXTURE3D_DESC textureDesc;
		ID3D11Texture3D* pTexture;
		// Initialize the  texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		// Setup the texture description.
		textureDesc.Width = resolution[0];
		textureDesc.Height = resolution[1];
		textureDesc.Depth = resolution[2];
		textureDesc.MipLevels = 1u;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		textureDesc.CPUAccessFlags = 0u;
		textureDesc.MiscFlags = 0u;

		INFOMAN(gfx);
		// Create the texture
		GFX_THROW_INFO(gfx.GetDevice()->CreateTexture3D(&textureDesc, NULL, &pTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		shaderResourceViewDesc.Texture3D.MostDetailedMip = 0u;
		shaderResourceViewDesc.Texture3D.MipLevels = textureDesc.MipLevels;

		// Create the shader resource view.
		GFX_THROW_INFO(gfx.GetDevice()->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, pSRV.GetAddressOf()));

		D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc;
		ZeroMemory(&unorderedAccessViewDesc, sizeof(unorderedAccessViewDesc));
		unorderedAccessViewDesc.Format = textureDesc.Format;
		unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
		unorderedAccessViewDesc.Texture3D.MipSlice = 0; // Specify the mip level
		unorderedAccessViewDesc.Texture3D.FirstWSlice = 0; // Specify the first depth slice to access
		unorderedAccessViewDesc.Texture3D.WSize = -1; // Use -1 to indicate the entire depth of the texture

		GFX_THROW_INFO(gfx.GetDevice()->CreateUnorderedAccessView(pTexture, &unorderedAccessViewDesc, pUAV.GetAddressOf()));
	}

	void Bind(Graphics& gfx) const override
	{
		gfx.GetContext()->VSSetShaderResources(1u, 1u, pNullSRV.GetAddressOf());
		gfx.GetContext()->CSSetUnorderedAccessViews(0u, 1u, pUAV.GetAddressOf(), nullptr);
	}
	void UnBind(Graphics& gfx) {
		gfx.GetContext()->CSSetUnorderedAccessViews(0u, 1u, pNullUAV.GetAddressOf(), nullptr);
	}
	void SetAsVSResource(Graphics& gfx) const {
		gfx.GetContext()->VSSetShaderResources(1u, 1u, pSRV.GetAddressOf());
	}

private:
	ComPtr<ID3D11ShaderResourceView> pSRV;
	ComPtr<ID3D11UnorderedAccessView> pUAV;
	ComPtr<ID3D11ShaderResourceView> pNullSRV;
	ComPtr<ID3D11UnorderedAccessView> pNullUAV;
};
