#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
public:
  Drawable() = default;
  Drawable(const Drawable&) = delete;
  virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
  void Draw(Graphics& gfx) const;
  virtual void Update(float dt) noexcept = 0;
  void AddBind(std::unique_ptr<Bindable> bind);
  void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
  virtual ~Drawable() = default;

private:
  const IndexBuffer* pIndexBuffer = nullptr;
  std::vector<std::unique_ptr<Bindable>> binds;
};