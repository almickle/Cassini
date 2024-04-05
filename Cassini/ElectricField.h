#pragma once
#include "Atom.h"
#include "Graphics.h"
#include "ResourceManager.h"
#include "Utility.h"

class ElectricField
{
public:
	ElectricField(Graphics& gfx, ResourceManager& manager, vector<Atom*> in_atoms)
		:atoms(in_atoms)
	{
		entityID = utility::GenerateUniqueID();
		instanceID = utility::GenerateUniqueID();
		manager.RegisterEntity(entityID);
		manager.RegisterInstance(entityID, instanceID);
		manager.CreateComputeShader(gfx, entityID, "FieldCS.cso");
		const UINT res[3] = { 100, 100, 100 };
		manager.CreateTexture3D(gfx, entityID, res);
		resourceID = utility::GenerateUniqueID();
		FieldBuffer fieldBuffer = {};
		for (int i = 0; i < in_atoms.size(); i++) {
			XMFLOAT3 pos = atoms[i]->GetPosition();
			fieldBuffer.particles[i] = { pos, atoms[i]->GetCharge() };
		}
		fieldBuffer.size = atoms.size();
		manager.CreateConstantBuffer(gfx, entityID, instanceID, resourceID, COMPUTE_SHADER_BUFFER, fieldBuffer);
	}

	void AddChargeContributor(Atom* atom) {
		atoms.push_back(atom);
	}

	void Bind(Graphics& gfx, ResourceManager& manager) {
		manager.BindStaticResources(gfx, entityID);
		manager.BindInstanceResources(gfx, entityID, instanceID);
	}
	void Dispatch(Graphics& gfx, ResourceManager& manager, UINT threadCount)
	{
		manager.Dispatch(gfx, entityID, threadCount);
		Texture3D* texture = reinterpret_cast<Texture3D*>(manager.GetStaticResourceByIndex(entityID, 1));
		texture->UnBind(gfx);
		texture->SetAsVSResource(gfx);
	};
private:
	string GetResourceID() {
		return resourceID;
	}
private:
	string entityID;
	string instanceID;
	string resourceID;
private:
	vector<Atom*> atoms;
};

