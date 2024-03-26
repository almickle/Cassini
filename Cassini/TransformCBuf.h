#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"

class TransformCBuf : public Bindable
{
public:
  TransformCBuf(Graphics& gfx, const Drawable& parent);
  void Bind(Graphics& gfx) noexcept override;

private:
  VertexConstantBuffer<XMMATRIX> vcbuf;
  const Drawable& parent;
};