#pragma once
#include "Bindables.h"
#include <unordered_map>

class ResourceManager
{
private:
	ResourceManager() {};
	static ResourceManager* instance;

public:
	ResourceManager(const ResourceManager&) = delete;
	static ResourceManager* Get() {
		if (instance == nullptr) {
			instance = new ResourceManager();
		}
		return instance;
	}
	void RegisterEntity(string entityID) {
		entities[entityID].staticResources = {};
		entities[entityID].instances = {};
	};

	void RegisterInstance(string entityID, string instanceID) {
		entities[entityID].instances[instanceID] = {};
	}
public:
	GraphicsResource* GetStaticResourceByIndex(string entityID, UINT index) {
		GraphicsResource* resource = entities.find(entityID)->second.staticResources[index];
		return resource;
	}

	InstanceResource* GetInstanceResourceByID(string entityID, string instanceID, string resourceID) {
		InstanceResource* resource = entities.find(entityID)->second.instances.find(instanceID)->second.resources.find(resourceID)->second;
		return resource;
	}

public:
	template<typename V>
	void CreateStaticResources(Graphics& gfx, string entityID, const vector<V>& vertices, const vector<unsigned short>& indices, string VSPath, string PSPath, D3D11_PRIMITIVE_TOPOLOGY topology) {
		CreateVertexBuffer(gfx, entityID, vertices);
		CreateIndexBuffer(gfx, entityID, indices);
		VertexShader* ref = CreateVertexShader(gfx, entityID, VSPath);
		CreatePixelShader(gfx, entityID, PSPath);
		CreatePrimitiveTopology(gfx, entityID, topology);
		CreateInputLayout(gfx, entityID, ref->GetBlob());
		CreateSampler(gfx, entityID);
	}

	void BindStaticResources(Graphics& gfx, string entityID) {
		for (int i = 0; i < entities[entityID].staticResources.size(); i++)
			entities[entityID].staticResources[i]->Bind(gfx);
	}

	void BindInstanceResources(Graphics& gfx, string entityID, string instanceID) {
		for (auto& resource : entities[entityID].instances[instanceID].resources)
		{
			resource.second->Bind(gfx);
		}
	}

	void BatchBindResources(Graphics& gfx, string entityID, ID3D11UnorderedAccessView* uavs[3]) {
		entities[entityID].staticResources[0]->Bind(gfx);
		gfx.GetContext()->CSSetUnorderedAccessViews(0u, 3u, uavs, nullptr);
	}

	void Dispatch(Graphics& gfx, string entityID, UINT threadGroupsX, UINT threadGroupsY, UINT threadGroupsZ) const {
		GraphicsResource* resource = entities.find(entityID)->second.staticResources[0];
		reinterpret_cast<ComputeShader*>(resource)->Execute(gfx, threadGroupsX, threadGroupsY, threadGroupsZ);
	}

public:
	// Static resource creation
	template<typename V>
	void CreateVertexBuffer(Graphics& gfx, string entityID, const vector<V>& vertices) {
		GraphicsResource* resource = new VertexBuffer<V>(gfx, vertices);
		entities[entityID].staticResources.push_back(resource);
	};
	void CreateIndexBuffer(Graphics& gfx, string entityID, const vector<unsigned short>& indices) {
		GraphicsResource* resource = new IndexBuffer(gfx, indices);
		entities[entityID].staticResources.push_back(resource);
	};
	void CreateInputLayout(Graphics& gfx, string entityID, ComPtr<ID3DBlob> pBlob) {
		GraphicsResource* resource = new InputLayout(gfx, pBlob);
		entities[entityID].staticResources.push_back(resource);
	};
	void CreatePrimitiveTopology(Graphics& gfx, string entityID, D3D11_PRIMITIVE_TOPOLOGY topology) {
		GraphicsResource* resource = new Topology(topology);
		entities[entityID].staticResources.push_back(resource);
	};
	VertexShader* CreateVertexShader(Graphics& gfx, string entityID, string path) {
		GraphicsResource* resource = new VertexShader(gfx, path);
		entities[entityID].staticResources.push_back(resource);
		return reinterpret_cast<VertexShader*>(resource);
	};
	void CreatePixelShader(Graphics& gfx, string entityID, string path) {
		GraphicsResource* resource = new PixelShader(gfx, path);
		entities[entityID].staticResources.push_back(resource);
	};
	void CreateRasterizer(Graphics& gfx, string entityID) {
		GraphicsResource* resource = new Rasterizer(gfx);
		entities[entityID].staticResources.push_back(resource);
	};
	void CreateComputeShader(Graphics& gfx, string entityID, string path) {
		GraphicsResource* resource = new ComputeShader(gfx, path);
		entities[entityID].staticResources.push_back(resource);
	}
	ComPtr<ID3D11ShaderResourceView> CreateTexture3D(Graphics& gfx, string entityID, const UINT res[3]) {
		GraphicsResource* resource = new Texture3D(gfx, res);
		entities[entityID].staticResources.push_back(resource);
		return reinterpret_cast<Texture3D*>(resource)->GetShaderResourceView();
	}
	void CreateSampler(Graphics& gfx, string entityID) {
		GraphicsResource* resource = new Sampler(gfx);
		entities[entityID].staticResources.push_back(resource);
	}

	template<typename Data>
	int CreateStructuredBuffer(Graphics& gfx, string entityID, const vector<Data>& data, UINT slot, bool input) {
		GraphicsResource* resource = new StructuredBuffer<Data>(gfx, data, slot, input);
		entities[entityID].staticResources.push_back(resource);
		return entities[entityID].staticResources.size() - 1;
	}

	template<typename T>
	void CreateConstantBuffer(Graphics& gfx, string entityID, string instanceID, string resourceID, UINT type, const T& cbData) {
		InstanceResource* resource = new ConstantBuffer<T>(gfx, type, cbData);
		entities[entityID].instances[instanceID].resources[resourceID] = resource;
	};

	template<typename T>
	int CreateStaticConstantBuffer(Graphics& gfx, string entityID, UINT slot, T& cbData) {
		GraphicsResource* resource = new StaticConstantBuffer<T>(gfx, slot, cbData);
		entities[entityID].staticResources.push_back(resource);
		return entities[entityID].staticResources.size() - 1;
	};

	void BindConstantBuffer(Graphics& gfx, string entityID, string instanceID, string resourceID, UINT slot) const {
		InstanceResource* resource = entities.find(entityID)->second.instances.find(instanceID)->second.resources.find(resourceID)->second;
		resource->Bind(gfx, slot);
	}

	template<typename T>
	void UpdateConstantData(Graphics& gfx, string entityID, string instanceID, string resourceID, const T& cbData) const {
		InstanceResource* resource = entities.find(entityID)->second.instances.find(instanceID)->second.resources.find(resourceID)->second;
		reinterpret_cast<ConstantBuffer<T>*>(resource)->Update(gfx, cbData);
	}

private:
	struct InstanceRecord {
		unordered_map<string, InstanceResource*> resources;
	};
	struct EntityRecord {
		vector<GraphicsResource*> staticResources;
		unordered_map<string, InstanceRecord> instances;
	};
	unordered_map<string, EntityRecord> entities;
};