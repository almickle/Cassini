#pragma once
#include "Buffer.h"
#include "Entity.h"
#include "Shader.h"
#include <vector>

using namespace std;

struct EntityRecord {
	vector<void*> shaders;
	vector<void*> buffers;
	vector<void*> instances;
};