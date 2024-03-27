#pragma once
#include "Camera.h"
#include "GDIPlusManager.h"
#include "Graphics.h"
#include "PointLight.h"
#include "Primitives.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include "Surface.h"

class Scene
{
public:
  Scene(Graphics& gfx);
  void UpdateScene(Graphics& gfx, ImVec2 size);

private:
  std::vector<std::unique_ptr<class Drawable>> drawables;
  static constexpr size_t nDrawables = 180;
  float frameCount = 0;
  float frameRate;
  float speedFactor = 1;
  float dt = 0.0f;
  Camera cam;
  PointLight light;
};
