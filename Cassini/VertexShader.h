#pragma once
#include "GraphicsResource.h"
#include "Utility.h"

class VertexShader : public GraphicsResource
{
public:
	VertexShader(Graphics& gfx, string path)
	{

		LPWSTR converted = utility::ConvertToLPWSTR(path);
		D3DReadFileToBlob(converted, &pBlob);
		gfx.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	};

	void Bind(Graphics& gfx) const override
	{
		gfx.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	ComPtr<ID3DBlob> GetBlob() { return pBlob; }

private:
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3DBlob> pBlob;
};