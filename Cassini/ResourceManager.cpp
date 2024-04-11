#include "Entity.h"
#include "ResourceManager.h"
#include "System.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() {};

ResourceManager* ResourceManager::Get() {
	if (instance == nullptr) {
		instance = new ResourceManager();
	}
	return instance;
}

// Entity bookkeeping
void ResourceManager::RegisterEntity(const string& entityID)
{
	registry.entities.emplace(entityID, EntityRecord{});
}
void ResourceManager::RegisterInstance(const string& entityID, Entity* instance)
{
	if (CheckForEntity(entityID))
		registry.entities.find(entityID)->second.instances.push_back(instance);
	else
		throw("Attempting to add instance to entity that does not exist");
}
unordered_map<string, EntityRecord> ResourceManager::GetEntityList() const
{
	return registry.entities;
}
EntityRecord ResourceManager::GetEntityRecord(const string& entityID) const
{
	return registry.entities.find(entityID)->second;
}
vector<Entity*> ResourceManager::GetInstances(const string& entityID) const
{
	return registry.entities.find(entityID)->second.instances;
}
Entity* ResourceManager::GetInstance(const string& entityID, UINT index) const
{
	return registry.entities.find(entityID)->second.instances[index];
}
UINT ResourceManager::GetInstanceCount(const string& entityID) const
{
	return registry.entities.find(entityID)->second.instances.size();
}
UINT ResourceManager::GetInstanceCount(const string& entityID, const string& systemID) const
{
	return registry.systems.find(systemID)->second.entities.find(entityID)->second.instances.size();
}
bool ResourceManager::CheckForEntity(const string& entityID) const
{
	if (registry.entities.count(entityID) > 0)
		return true;
	else
		return false;
}
bool ResourceManager::IsStaticInitialized(const string& entityID) const
{
	return GetInstanceCount(entityID) > 1u;
}
bool ResourceManager::IsStaticInitialized(const string& entityID, const string& systemID) const
{
	return GetInstanceCount(entityID, systemID) > 1u;
}

// System bookkeeping
void ResourceManager::RegisterSystem(const string& systemID)
{
	registry.systems.emplace(systemID, SystemRecord{});
}
void ResourceManager::RegisterSystemInstance(const string& systemID, System* instance)
{
	if (CheckForSystem(systemID))
		registry.systems.find(systemID)->second.instances.push_back(instance);
	else
		throw("Attempting to add instance to a system that does not exist");
}
unordered_map<string, SystemRecord> ResourceManager::GetSystemList() const
{
	return registry.systems;
}
unordered_map<string, EntityRecord> ResourceManager::GetSystemEntityList(const string& systemID) const
{
	return registry.systems.find(systemID)->second.entities;
}
void ResourceManager::RegisterEntityToSystem(const string& systemID, const string& entityID)
{
	registry.systems.find(systemID)->second.entities.emplace(entityID, EntityRecord{});
}
void ResourceManager::RegisterEntityInstanceToSystem(const string& systemID, const string& entityID, Entity* instance)
{
	registry.systems.find(systemID)->second.entities.find(entityID)->second.instances.push_back(instance);
}
bool ResourceManager::ResourceManager::CheckForSystem(const string& systemID) const
{
	if (registry.systems.count(systemID) > 0)
		return true;
	else
		return false;
}
bool ResourceManager::CheckForEntityInSystem(const string& systemID, const string& entityID)
{
	if (registry.systems.find(systemID)->second.entities.count(entityID) > 0)
		return true;
	else
		return false;
}
vector<System*> ResourceManager::GetSystemInstances(const string& systemID) const
{
	return registry.systems.find(systemID)->second.instances;
}


// Resource binding
void ResourceManager::BindGlobalBuffers(Graphics& gfx) const
{
	for (auto& buffer : registry.globalBuffers)
	{
		buffer->Bind(gfx);
	}
}
void ResourceManager::BindResources(Graphics& gfx, const string& recordID, const RecordType& recordType) const
{
	BindShaders(gfx, recordID, recordType);
	BindBuffers(gfx, recordID, recordType);
}
void ResourceManager::BindResources(Graphics& gfx, const string& systemID, const string& entityID) const
{
	BindShaders(gfx, systemID, entityID);
	BindBuffers(gfx, systemID, entityID);
}
void ResourceManager::BindShaders(Graphics& gfx, const string& recordID, RecordType recordType) const
{
	switch (recordType)
	{
	case ENTITY:
	{
		for (auto& shader : registry.entities.find(recordID)->second.shaders)
			shader->Bind(gfx);
	}
	break;
	case SYSTEM:
	{
		for (auto& shader : registry.systems.find(recordID)->second.systemShaders)
			shader->Bind(gfx);
	}
	break;
	}
}

void ResourceManager::BindBuffers(Graphics& gfx, const string& recordID, RecordType recordType) const
{
	switch (recordType)
	{
	case ENTITY:
	{
		for (auto& buffer : registry.entities.find(recordID)->second.buffers)
			buffer->Bind(gfx);
	}
	break;
	case SYSTEM:
	{
		for (auto& buffer : registry.systems.find(recordID)->second.systemBuffers)
			buffer->Bind(gfx);
	}
	break;
	}
}
void ResourceManager::BindBuffers(Graphics& gfx, const string& systemID, const string& entityID) const
{
	for (auto& buffer : registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers)
		buffer->Bind(gfx);
}
void ResourceManager::BindShaders(Graphics& gfx, const string& systemID, const string& entityID) const
{
	for (auto& shader : registry.systems.find(systemID)->second.entities.find(entityID)->second.shaders)
		shader->Bind(gfx);
}


void ResourceManager::DrawEntityInstances(Graphics& gfx, const string& entityID)
{
	EntityRecord entity = registry.entities.find(entityID)->second;
	gfx.DrawInstancedIndexed(entity.indexCount, entity.instances.size());
}

void ResourceManager::DrawSystemEntityInstances(Graphics& gfx, const string& systemID, const string& entityID)
{
	EntityRecord entity = registry.systems.find(systemID)->second.entities.find(entityID)->second;
	gfx.DrawInstancedIndexed(entity.indexCount, entity.instances.size());
}



// Resource retrieval
Buffer* ResourceManager::GetGlobalBufferByIndex(const string& entityID, UINT index) const
{
	return registry.globalBuffers[index];
}
Buffer* ResourceManager::GetBufferByIndex(const string& entityID, UINT index) const
{
	Buffer* buffer = registry.entities.find(entityID)->second.buffers[index];
	return buffer;
}
Buffer* ResourceManager::GetBufferByIndex(const string& systemID, const string& entityID, UINT index) const
{
	Buffer* buffer = registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers[index];
	return buffer;
}
Buffer* ResourceManager::GetSystemBufferByIndex(const string& systemID, UINT index) const
{
	Buffer* buffer = registry.systems.find(systemID)->second.systemBuffers[index];
	return buffer;
}

Shader* ResourceManager::GetShaderByIndex(const string& entityID, UINT index) const
{
	Shader* shader = registry.entities.find(entityID)->second.shaders[index];
	return shader;
}

UINT ResourceManager::CreateIndexBuffer(Graphics& gfx, const string& entityID, const vector<unsigned short>& indices)
{
	Buffer* buffer = new IndexBuffer(gfx, indices);
	registry.entities.find(entityID)->second.buffers.push_back(buffer);
	registry.entities.find(entityID)->second.indexCount = indices.size();
	return registry.entities.find(entityID)->second.buffers.size() - 1;
}
UINT ResourceManager::CreateIndexBuffer(Graphics& gfx, const string& entityID, const string& systemID, const vector<unsigned short>& indices)
{
	Buffer* buffer = new IndexBuffer(gfx, indices);
	registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers.push_back(buffer);
	registry.systems.find(systemID)->second.entities.find(entityID)->second.indexCount = indices.size();
	return registry.systems.find(systemID)->second.entities.find(entityID)->second.buffers.size() - 1;
}

// Shader creation
void ResourceManager::CreateVertexShader(Graphics& gfx, const string& recordID, const string& path, const vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc)
{
	Shader* shader = new VertexShader(gfx, path, layoutDesc);
	registry.entities.find(recordID)->second.shaders.push_back(shader);
}
void ResourceManager::CreateVertexShader(Graphics& gfx, const string& entityID, const string& systemID, const string& path, const vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc)
{
	Shader* shader = new VertexShader(gfx, path, layoutDesc);
	registry.systems.find(systemID)->second.entities.find(entityID)->second.shaders.push_back(shader);
}
void ResourceManager::CreatePixelShader(Graphics& gfx, const string& recordID, const string& path)
{
	Shader* shader = new PixelShader(gfx, path);
	registry.entities.find(recordID)->second.shaders.push_back(shader);
}
void ResourceManager::CreatePixelShader(Graphics& gfx, const string& entityID, const string& systemID, const string& path)
{
	Shader* shader = new PixelShader(gfx, path);
	registry.systems.find(systemID)->second.entities.find(entityID)->second.shaders.push_back(shader);
}

void ResourceManager::CreateComputeShader(Graphics& gfx, const string& recordID, const string& path, const RecordType& recordType)
{
	Shader* shader = new ComputeShader(gfx, path);
	switch (recordType)
	{
	case ENTITY:
	{
		registry.entities.find(recordID)->second.shaders.push_back(shader);
	}
	break;
	case SYSTEM:
	{
		registry.systems.find(recordID)->second.systemShaders.push_back(shader);
	}
	break;
	}
}

void ResourceManager::SpawnControlWindow() {
	ImGui::Begin("Resource Manager");
	if (ImGui::TreeNode("Systems")) {
		for (auto& system : registry.systems) {
			if (ImGui::TreeNode(system.first.c_str())) {
				if (ImGui::TreeNode("Shaders"))
				{
					for (auto& shader : system.second.systemShaders) {
						ImGui::Text("Shader");
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Buffers"))
				{
					for (auto& buffer : system.second.systemBuffers) {
						ImGui::Text("Buffer");
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Entities"))
				{
					for (auto& entity : system.second.entities) {
						if (ImGui::TreeNode(entity.first.c_str())) {
							if (ImGui::TreeNode("Shaders")) {
								for (auto& shader : entity.second.shaders) {
									ImGui::Text("Shader");
								}
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Buffers"))
							{
								for (auto& buffer : entity.second.buffers) {
									ImGui::Text("Buffer");
								}
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Instances"))
							{
								for (int i = 0; i < entity.second.instances.size(); i++) {
									ImGui::Text(to_string(i).c_str());
								}
								ImGui::TreePop();
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Entities"))
	{
		for (auto& entity : registry.entities)
		{
			if (ImGui::TreeNode(entity.first.c_str()))
			{
				if (ImGui::TreeNode("Shaders"))
				{
					for (auto& shader : entity.second.shaders) {
						ImGui::Text("Shader");
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Buffers"))
				{
					for (auto& buffer : entity.second.buffers) {
						ImGui::Text("Buffer");
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Instances"))
				{
					for (int i = 0; i < entity.second.instances.size(); i++) {
						ImGui::Text(to_string(i).c_str());
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}