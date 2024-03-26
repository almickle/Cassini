#include "Bindables.h"
#include "Cube.h"
#include "GraphicsThrowMacros.h"

Cube::Cube(Graphics& gfx,
           mt19937& rng,
           uniform_real_distribution<float>& adist,
           uniform_real_distribution<float>& ddist,
           uniform_real_distribution<float>& odist,
           uniform_real_distribution<float>& rdist)
  : r(rdist(rng))
  , droll(ddist(rng))
  , dpitch(ddist(rng))
  , dyaw(ddist(rng))
  , dphi(odist(rng))
  , dtheta(odist(rng))
  , dchi(odist(rng))
  , chi(adist(rng))
  , theta(adist(rng))
  , phi(adist(rng))
{
  struct Vertex
  {
    struct
    {
      float x;
      float y;
      float z;
    } pos;
  };
  const vector<Vertex> vertices = {
    { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f },
    { 1.0f, 1.0f, -1.0f },   { -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f },   { 1.0f, 1.0f, 1.0f },
  };
  AddBind(make_unique<VertexBuffer>(gfx, vertices));

  auto pvs = make_unique<VertexShader>(gfx, L"VertexShader.cso");
  auto pvsbc = pvs->GetBytecode();
  AddBind(move(pvs));

  AddBind(make_unique<PixelShader>(gfx, L"PixelShader.cso"));

  const vector<unsigned short> indices = { 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5,
                                           2, 6, 3, 3, 6, 7, 4, 5, 7, 4, 7, 6,
                                           0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 };
  AddIndexBuffer(make_unique<IndexBuffer>(gfx, indices));

  struct ConstantBuffer2
  {
    struct
    {
      float r;
      float g;
      float b;
      float a;
    } face_colors[6];
  };
  const ConstantBuffer2 cb2 = { {
    { 1.0f, 0.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f, 1.0f },
  } };
  AddBind(make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

  const vector<D3D11_INPUT_ELEMENT_DESC> ied = {
    { "Position",
      0,
      DXGI_FORMAT_R32G32B32_FLOAT,
      0,
      0,
      D3D11_INPUT_PER_VERTEX_DATA,
      0 },
  };
  AddBind(make_unique<InputLayout>(gfx, ied, pvsbc));

  AddBind(make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

  AddBind(make_unique<TransformCBuf>(gfx, *this));
}

void
Cube::Update(float dt) noexcept
{
  roll += droll * dt;
  pitch += dpitch * dt;
  yaw += dyaw * dt;
  theta += dtheta * dt;
  phi += dphi * dt;
  chi += dchi * dt;
}

XMMATRIX
Cube::GetTransformXM() const noexcept
{
  return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
         XMMatrixTranslation(r, 0.0f, 0.0f) *
         XMMatrixRotationRollPitchYaw(theta, phi, chi) *
         XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}