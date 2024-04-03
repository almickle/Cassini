#pragma once
#include "Windows.h"

struct Vertex
{
	struct Position
	{
		float x;
		float y;
		float z;
	} position;
	struct Normal
	{
		float x;
		float y;
		float z;
	} normal;
	struct TextureCoordinate
	{
		float u;
		float v;
	} textureCoordinate;
};

struct Vec3 {
	float x;
	float y;
	float z;
};

struct Vec2 {
	float x;
	float y;
};

struct Resolution {
	UINT width;
	UINT height;
};