#pragma once
#include "Bindables.h"
#include "Entity.h"
#include <unordered_map>

struct EntityRecord {
	vector<Shader*> shaders;
	vector<Buffer*> buffers;
	vector<Entity*> instances;
};

class ResourceManager
{
private:
	ResourceManager();
	static ResourceManager* instance;
public:
	ResourceManager(const ResourceManager&) = delete;
	static ResourceManager* Get();
	void RegisterEntity(const string& entityID);
	void RegisterInstance(const string& entityID, Entity* instance);
public:
	unordered_map<string, EntityRecord> GetRegistry() const;
	EntityRecord GetEntityRecord(const string& entityID) const;
	Entity* GetInstance(const string& entityID, UINT index) const;
	UINT GetInstanceCount(const string& entityID) const;
	bool CheckForEntity(string entityID) const;
public:
	void DrawInstances(Graphics& gfx, const string& entityID);
	void Dispatch(Graphics& gfx, const string& entityID, UINT threadGroupsX, UINT threadGroupsY, UINT threadGroupsZ) const;
	void SpawnControlWindow();
public:
	template<typename V>
	UINT CreateVertexBuffer(Graphics& gfx, const string& entityID, const vector<V>& vertices, UINT slot);
	template<typename T>
	UINT CreateInputStructuredBuffer(Graphics& gfx, const string& entityID, const vector<T>& data, ShaderDataTypes type, UINT slot);
	template<typename T>
	UINT CreateOutputStructuredBuffer(Graphics& gfx, const string& entityID, const vector<T>& data, UINT slot);
	template<typename T>
	UINT CreateDynamicConstantBuffer(Graphics& gfx, const string& entityID, const T& cbData, ShaderDataTypes type, UINT slot);
	template<typename T>
	UINT CreateStaticConstantBuffer(Graphics& gfx, const string& entityID, const T& data, UINT slot);
	UINT CreateIndexBuffer(Graphics& gfx, const string& entityID, const vector<unsigned short>& indices);
public:
	void CreateVertexShader(Graphics& gfx, const string& entityID, const string& path, const vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc);
	void CreatePixelShader(Graphics& gfx, const string& entityID, const string& path);
	void CreateComputeShader(Graphics& gfx, const string& entityID, const string& path);
public:
	void BindShaders(Graphics& gfx, const string& entityID) const;
	void BindBuffers(Graphics& gfx, const string& entityID) const;
public:
	void UpdateBuffer(Graphics& gfx, const string& entityID, UINT index);
public:
	Buffer* GetBufferByIndex(string entityID, UINT index) const;
	Shader* GetShaderByIndex(string entityID, UINT index) const;
private:
	unordered_map<string, EntityRecord> registry;
};