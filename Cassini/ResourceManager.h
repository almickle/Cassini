#pragma once
#include "Buffers.h"
#include "Shaders.h"
#include <unordered_map>

class Entity;
class System;

enum RecordType {
	ENTITY = 0,
	SYSTEM
};

struct EntityRecord {
	vector<Shader*> shaders;
	vector<Buffer*> buffers;
	vector<Entity*> instances;
	UINT vertexCount;
	UINT indexCount;
};

struct SystemRecord {
	unordered_map<string, EntityRecord> entities;
	vector<System*> instances;
	vector<Shader*> systemShaders;
	vector<Buffer*> systemBuffers;
};

struct Registry {
	unordered_map<string, EntityRecord> entities;
	unordered_map<string, SystemRecord> systems;
	vector<Buffer*> globalBuffers;
};

class ResourceManager
{
private:
	ResourceManager();
	static ResourceManager* instance;
public:
	ResourceManager(const ResourceManager&) = delete;
	static ResourceManager* Get();
public:
	void RegisterEntity(const string& entityID);
	void RegisterInstance(const string& entityID, Entity* instance);
	unordered_map<string, EntityRecord> GetEntityList() const;
	EntityRecord GetEntityRecord(const string& entityID) const;
	vector<Entity*> GetInstances(const string& entityID) const;
	Entity* GetInstance(const string& entityID, UINT index) const;
	UINT GetInstanceCount(const string& entityID) const;
	UINT GetInstanceCount(const string& entityID, const string& systemID) const;
	bool CheckForEntity(const string& entityID) const;
	bool IsStaticInitialized(const string& entityID) const;
	bool IsStaticInitialized(const string& entityID, const string& systemID) const;
public:
	void RegisterSystem(const string& systemID);
	void RegisterSystemInstance(const string& systemID, System* instance);
	vector<System*> GetSystemInstances(const string& systemID) const;
	unordered_map<string, SystemRecord> GetSystemList() const;
	unordered_map<string, EntityRecord> GetSystemEntityList(const string& systemID) const;
	bool CheckForSystem(const string& systemID) const;
	bool CheckForEntityInSystem(const string& systemID, const string& entityID);
	void RegisterEntityToSystem(const string& systemID, const string& entityID);
	void RegisterEntityInstanceToSystem(const string& systemID, const string& entityID, Entity* instance);
public:
	Buffer* GetGlobalBufferByIndex(const string& entityID, UINT index) const;
	Buffer* GetBufferByIndex(const string& entityID, UINT index) const;
	Buffer* GetBufferByIndex(const string& systemID, const string& entityID, UINT index) const;
	Buffer* GetSystemBufferByIndex(const string& systemID, UINT index) const;
	Shader* GetShaderByIndex(const string& entityID, UINT index) const;
public:
	void DrawEntityInstances(Graphics& gfx, const string& entityID);
	void DrawSystemEntityInstances(Graphics& gfx, const string& systemID, const string& entityID);
	void SpawnControlWindow();
public:
	void CreateVertexShader(Graphics& gfx, const string& entityID, const string& path, const vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc);
	void CreateVertexShader(Graphics& gfx, const string& entityID, const string& systemID, const string& path, const vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc);
	void CreatePixelShader(Graphics& gfx, const string& entityID, const string& path);
	void CreatePixelShader(Graphics& gfx, const string& entityID, const string& systemID, const string& path);
	void CreateComputeShader(Graphics& gfx, const string& recordID, const string& path, const RecordType& recordType);
public:
	void BindGlobalBuffers(Graphics& gfx) const;
	void BindResources(Graphics& gfx, const string& recordID, const RecordType& recordType) const;
	void BindResources(Graphics& gfx, const string& systemID, const string& entityID) const;
	void BindShaders(Graphics& gfx, const string& recordID, RecordType recordType) const;
	void BindShaders(Graphics& gfx, const string& systemID, const string& entityID) const;
	void BindBuffers(Graphics& gfx, const string& recordID, RecordType recordType) const;
	void BindBuffers(Graphics& gfx, const string& systemID, const string& entityID) const;
public:
	template<typename T>
	UINT CreateGlobalStaticConstantBuffer(Graphics& gfx, const T& data, UINT slot)
	{
		Buffer* buffer = new StaticConstantBuffer<T>(gfx, data, slot);
		registry.globalBuffers.push_back(buffer);
		return registry.globalBuffers.size() - 1;
	}
	template<typename T>
	UINT CreateGlobalDynamicConstantBuffer(Graphics& gfx, const T& data, ShaderDataTypes type, UINT slot)
	{
		Buffer* buffer = new DynamicConstantBuffer<T>(gfx, data, type, slot);
		registry.globalBuffers.push_back(buffer);
		return registry.globalBuffers.size() - 1;
	}
public:
	UINT CreateIndexBuffer(Graphics& gfx, const string& entityID, const vector<unsigned short>& indices);
	UINT CreateIndexBuffer(Graphics& gfx, const string& entityID, const string& systemID, const vector<unsigned short>& indices);
	template<typename V>
	UINT CreateVertexBuffer(Graphics& gfx, const string& entityID, const vector<V>& vertices, UINT slot)
	{
		Buffer* buffer = new VertexBuffer<V>(gfx, vertices, slot);
		registry.entities.find(entityID)->second.buffers.push_back(buffer);
		registry.entities.find(entityID)->second.vertexCount = vertices.size();
		return registry.entities.find(entityID)->second.buffers.size() - 1;
	}
	template<typename V>
	UINT CreateVertexBuffer(Graphics& gfx, const string& entityID, const string& systemID, const vector<V>& vertices, UINT slot)
	{
		Buffer* buffer = new VertexBuffer<V>(gfx, vertices, slot);
		registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers.push_back(buffer);
		registry.systems.find(systemID)->second.entities.find(entityID)->second.vertexCount = vertices.size();
		return registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers.size() - 1;
	}
public:
	template<typename T>
	UINT CreateInputStructuredBuffer(Graphics& gfx, const string& recordID, const vector<T>& data, ShaderDataTypes shaderType, UINT slot, RecordType recordType)
	{
		Buffer* buffer = new InputStructuredBuffer<T>(gfx, data, shaderType, slot);
		switch (recordType)
		{
		case RecordType::ENTITY:
		{
			vector<Buffer*>& buffers = registry.entities.find(recordID)->second.buffers;
			buffers.push_back(buffer);
			return buffers.size() - 1;
		}
		case RecordType::SYSTEM:
		{
			vector<Buffer*>& buffers = registry.systems.find(recordID)->second.systemBuffers;
			buffers.push_back(buffer);
			return buffers.size() - 1;
		}
		}
	}
	template<typename T>
	UINT CreateInputStructuredBuffer(Graphics& gfx, const string& entityID, const string& systemID, const vector<T>& data, ShaderDataTypes shaderType, UINT slot)
	{
		Buffer* buffer = new InputStructuredBuffer<T>(gfx, data, shaderType, slot);
		vector<Buffer*>& buffers = registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers;
		buffers.push_back(buffer);
		return buffers.size() - 1;
	}
	template<typename T>
	UINT CreateOutputStructuredBuffer(Graphics& gfx, const string& recordID, const vector<T>& data, UINT slot, RecordType recordType)
	{
		Buffer* buffer = new OutputStructuredBuffer<T>(gfx, data, slot);
		switch (recordType)
		{
		case ENTITY:
		{
			vector<Buffer*>& buffers = registry.entities.find(recordID)->second.buffers;
			buffers.push_back(buffer);
			return buffers.size() - 1;
		}
		case SYSTEM:
		{
			vector<Buffer*>& buffers = registry.systems.find(recordID)->second.systemBuffers;
			buffers.push_back(buffer);
			return buffers.size() - 1;
		}
		}
	}
	template<typename T>
	UINT CreateDynamicConstantBuffer(Graphics& gfx, const string& recordID, const T& data, ShaderDataTypes shaderType, UINT slot, RecordType recordType)
	{
		Buffer* buffer = new DynamicConstantBuffer<T>(gfx, data, shaderType, slot);
		switch (recordType)
		{
		case ENTITY:
		{
			vector<Buffer*>& buffers = registry.entities.find(recordID)->second.buffers;
			buffers.push_back(buffer);
			return buffers.size() - 1;
		}
		case SYSTEM:
		{
			vector<Buffer*>& buffers = registry.systems.find(recordID)->second.systemBuffers;
			buffers.push_back(buffer);
			return buffers.size() - 1;
		}
		}
	}
	template<typename T>
	UINT CreateStaticConstantBuffer(Graphics& gfx, const string& recordID, const T& data, ShaderDataTypes shaderType, UINT slot, RecordType recordType)
	{
		Buffer* buffer = new StaticConstantBuffer<T>(gfx, data, shaderType, slot);
		switch (recordType)
		{
		case ENTITY:
		{
			vector<Buffer*>& buffers = registry.entities.find(recordID)->second.buffers;
			buffers.push_back(buffer);
			return buffers.size() - 1;
		}
		case SYSTEM:
		{
			vector<Buffer*>& buffers = registry.systems.find(recordID)->second.systemBuffers;
			buffers.push_back(buffer);
			return buffers.size() - 1;
		}
		}
	}
	template<typename T>
	UINT CreateStaticConstantBuffer(Graphics& gfx, const string& entityID, const string& systemID, const T& data, ShaderDataTypes shaderType, UINT slot, RecordType recordType)
	{
		Buffer* buffer = new StaticConstantBuffer<T>(gfx, data, shaderType, slot);
		vector<Buffer*>& buffers = registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers;
		buffers.push_back(buffer);
		return buffers.size() - 1;
	}
public:
	template<typename T>
	void UpdateGlobalBuffer(Graphics& gfx, UINT index, const T& data)
	{
		DynamicConstantBuffer<T>* buffer = reinterpret_cast<DynamicConstantBuffer<T>*>(registry.globalBuffers[index]);
		buffer->Update(gfx, data);
	}
	template<typename T>
	void UpdateStructuredBuffer(Graphics& gfx, const string& entityID, UINT index, const vector<T>& data)
	{
		InputStructuredBuffer<T>* buffer = reinterpret_cast<InputStructuredBuffer<T>*>(registry.entities.find(entityID)->second.buffers[index]);
		buffer->WriteToBuffer(gfx, data);
	}
	template<typename T>
	void UpdateStructuredBuffer(Graphics& gfx, const string& systemID, const string& entityID, UINT index, const vector<T>& data)
	{
		InputStructuredBuffer<T>* buffer = reinterpret_cast<InputStructuredBuffer<T>*>(registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers[index]);
		buffer->WriteToBuffer(gfx, data);
	}
	template<typename T>
	void UpdateConstantBuffer(Graphics& gfx, const string& entityID, UINT index, const T& data)
	{
		DynamicConstantBuffer<T>* buffer = reinterpret_cast<DynamicConstantBuffer<T>*>(registry.entities.find(entityID)->second.buffers[index]);
		buffer->Update(gfx, data);
	}
	template<typename T>
	void UpdateSystemStructuredBuffer(Graphics& gfx, const string& systemID, UINT index, const vector<T>& data)
	{
		InputStructuredBuffer<T>* buffer = reinterpret_cast<InputStructuredBuffer<T>*>(registry.systems.find(systemID)->second.systemBuffers[index]);
		buffer->WriteToBuffer(gfx, data);
	}
	template<typename T>
	void UpdateSystemConstantBuffer(Graphics& gfx, const string& systemID, UINT index, const T& data)
	{
		DynamicConstantBuffer<T>* buffer = reinterpret_cast<DynamicConstantBuffer<T>*>(registry.systems.find(systemID)->second.systemBuffers[index]);
		buffer->Update(gfx, data);
	}
private:
	Registry registry;
};