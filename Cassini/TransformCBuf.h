#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"

class TransformCBuf : public Bindable
{
public:
  TransformCBuf(Graphics& gfx, const Drawable& parent);
  void Bind(Graphics& gfx) noexcept override;

private:
  static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf;
  const Drawable& parent;
};