#pragma once
#include "Graphics.h"

class PixelShader
{
public:
	PixelShader( Graphics& gfx )
	{
		D3DReadFileToBlob( L"PixelShader.cso", &pBlob );
		gfx.GetDevice()->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader );
	};

	void Bind( Graphics& gfx )
	{
		gfx.GetContext()->PSSetShader( pPixelShader.Get(), nullptr, 0u );
	}

	ComPtr<ID3DBlob> GetBlob() { return pBlob; };


private:
	ComPtr<ID3D11PixelShader> pPixelShader;
	ComPtr<ID3DBlob> pBlob;
};