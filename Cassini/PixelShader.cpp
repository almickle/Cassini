#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, string path)
{
	LPWSTR converted = utility::ConvertToLPWSTR(path);
	D3DReadFileToBlob(converted, &pBlob);
	gfx.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
};

void PixelShader::Bind(Graphics& gfx) const
{
	gfx.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}