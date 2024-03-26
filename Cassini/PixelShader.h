#pragma once
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
  PixelShader(Graphics& gfx, const wstring& path);
  void Bind(Graphics& gfx) noexcept override;

protected:
  ComPtr<ID3D11PixelShader> pPixelShader;
};