#pragma once
#include "Graphics.h"
#include "Primitives.h"

class Scene
{
public:
  Scene(Graphics& gfx);
  void UpdateScene(Graphics& gfx, ImVec2 size);
  void DrawScene(Graphics& gfx);

private:
  vector<unique_ptr<class Cube>> cubes;

  float frameCount = 0;
};
