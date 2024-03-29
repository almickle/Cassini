#pragma once
#include "Graphics.h"

class VertexShader
{
public:
	VertexShader( Graphics& gfx )
	{
		D3DReadFileToBlob( L"VertexShader.cso", &pBlob );
		gfx.GetDevice()->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader );
	};

	void Bind( Graphics& gfx )
	{
		gfx.GetContext()->VSSetShader( pVertexShader.Get(), nullptr, 0u );
	}

	ComPtr<ID3DBlob> GetBlob() { return pBlob; }


private:
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3DBlob> pBlob;
};