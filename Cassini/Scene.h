#pragma once
#include "Graphics.h"
#include "Primitives.h"

class Scene
{
public:
  Scene(Graphics* pGfx);
  void UpdateScene();
  void DrawScene();

private:
  vector<unique_ptr<class Cube>> cubes;
  Graphics* gfx;

  float frameCount = 0;
};
