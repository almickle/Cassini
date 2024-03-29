#pragma once

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