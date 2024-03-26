#pragma once
#include "Drawable.h"

class Cube : public Drawable
{
public:
  Cube(Graphics& gfx,
       mt19937& rng,
       uniform_real_distribution<float>& adist,
       uniform_real_distribution<float>& ddist,
       uniform_real_distribution<float>& odist,
       uniform_real_distribution<float>& rdist);
  void Update(float dt) noexcept override;
  XMMATRIX GetTransformXM() const noexcept override;

private:
  // positional
  float r;
  float roll = 0.0f;
  float pitch = 0.0f;
  float yaw = 0.0f;
  float theta;
  float phi;
  float chi;
  // speed (delta/s)
  float droll;
  float dpitch;
  float dyaw;
  float dtheta;
  float dphi;
  float dchi;
};