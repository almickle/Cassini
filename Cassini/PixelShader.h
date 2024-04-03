#pragma once
#include "GraphicsResource.h"
#include "Utility.h"

class PixelShader : public GraphicsResource
{
public:
	PixelShader(Graphics& gfx, string path)
	{
		LPWSTR converted = utility::ConvertToLPWSTR(path);
		D3DReadFileToBlob(converted, &pBlob);
		gfx.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	};

	void Bind(Graphics& gfx) const override
	{
		gfx.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

	ComPtr<ID3DBlob> GetBlob() { return pBlob; }


private:
	ComPtr<ID3D11PixelShader> pPixelShader;
	ComPtr<ID3DBlob> pBlob;
};