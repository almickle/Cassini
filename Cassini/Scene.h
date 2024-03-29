#pragma once
#include "Entity.h"
#include "GDIPlusManager.h"
#include "Graphics.h"
#include <algorithm>
#include <chrono>
#include <cmath>

class Scene
{
public:
	Scene( Graphics& gfx );
	void UpdateScene( Graphics& gfx, ImVec2 size );
	void AddEntity( Entity& entity );

private:
	vector<Entity> entities;
private:
	float frameCount = 0;
	float frameRate = 0;
	float speedFactor = 1;
	float dt = 0.0f;
};
