#pragma once
#include "GDIPlusManager.h"
#include "Graphics.h"
#include <algorithm>
#include <chrono>
#include <cmath>

class Scene
{
public:
  Scene(Graphics& gfx);
  void UpdateScene(Graphics& gfx, ImVec2 size);

private:
  float frameCount = 0;
  float frameRate;
  float speedFactor = 1;
  float dt = 0.0f;
};
