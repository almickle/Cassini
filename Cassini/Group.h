#pragma once
#include "Entity.h"

class Group
{
public:
	Group(vector<Entity*> in_entities, XMFLOAT3 in_centroid, vector<XMFLOAT3> in_layout) : entities(in_entities)
	{
		entities = in_entities;
		centroid = in_centroid;
		layout = in_layout;
		for (int i = 0; i < entities.size(); i++) {
			XMFLOAT3 pos;
			XMStoreFloat3(&pos, XMLoadFloat3(&centroid) + XMLoadFloat3(&layout[i]));
			entities[i]->SetPosition(pos);
		}
	}
	void Update(Graphics& gfx, ResourceManager& manager) {
		for (auto& entity : entities)
			entity->Update(gfx, manager, 1.0f);
	}
private:
	vector<Entity*> entities;
	vector<XMFLOAT3> layout;
	XMFLOAT3 centroid;
};

