#include "VertexShader.h"

VertexShader::VertexShader(Graphics& gfx, const string& path, const vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc)
{
	LPWSTR converted = utility::ConvertToLPWSTR(path);
	D3DReadFileToBlob(converted, &pBlob);
	gfx.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	gfx.GetDevice()->CreateInputLayout(layoutDesc.data(), size(layoutDesc), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), pInputLayout.GetAddressOf());
}

void VertexShader::Bind(Graphics& gfx) const
{
	gfx.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	gfx.GetContext()->IASetInputLayout(pInputLayout.Get());
	gfx.GetContext()->IASetPrimitiveTopology(topology);
}

void VertexShader::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type)
{
	topology = type;
}
