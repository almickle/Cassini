#include "GraphicsThrowMacros.h"
#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
  INFOMAN(gfx);

  ComPtr<ID3DBlob> pBlob;
  D3DReadFileToBlob(path.c_str(), &pBlob);
  GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(
    pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
}

void
PixelShader::Bind(Graphics& gfx) noexcept
{
  GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}