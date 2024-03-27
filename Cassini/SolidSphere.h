#pragma once
#include "Primitive.h"

class SolidSphere : public Primitive<SolidSphere>
{
public:
  SolidSphere(Graphics& gfx, float radius);
  void Update(float dt) noexcept override;
  void SetPos(DirectX::XMFLOAT3 pos) noexcept;
  DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
  DirectX::XMFLOAT3 pos = { 1.0f, 1.0f, 1.0f };
};