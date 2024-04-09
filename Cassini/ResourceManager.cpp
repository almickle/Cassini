#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() {};

ResourceManager* ResourceManager::Get() {
	if (instance == nullptr) {
		instance = new ResourceManager();
	}
	return instance;
}

void ResourceManager::RegisterEntity(const string& entityID)
{
	registry.emplace(entityID, EntityRecord{});
}

void ResourceManager::RegisterInstance(const string& entityID, Entity* entity)
{
	if (CheckForEntity(entityID))
		registry.find(entityID)->second.instances.push_back({});
	else
		throw("Attempting to add instance to entity that does not exist");
}

unordered_map<string, EntityRecord> ResourceManager::GetRegistry() const
{
	return registry;
}

EntityRecord ResourceManager::GetEntityRecord(const string& entityID) const
{
	return registry.find(entityID)->second;
}

Entity* ResourceManager::GetInstance(const string& entityID, UINT index) const
{
	return registry.find(entityID)->second.instances[index];
}


UINT ResourceManager::GetInstanceCount(const string& entityID) const
{
	return registry.find(entityID)->second.instances.size();
}

bool ResourceManager::CheckForEntity(string entityID) const
{
	if (registry.count(entityID) > 0)
		return true;
	else
		return false;
}

// Resource retrieval
Buffer* ResourceManager::GetBufferByIndex(string entityID, UINT index) const
{
	Buffer* buffer = registry.find(entityID)->second.buffers[index];
	return buffer;
}

Shader* ResourceManager::GetShaderByIndex(string entityID, UINT index) const
{
	Shader* shader = registry.find(entityID)->second.shaders[index];
	return shader;
}

// Resource binding
void ResourceManager::BindShaders(Graphics& gfx, const string& entityID) const
{
	for (auto& shader : registry.find(entityID)->second.shaders)
		shader->Bind(gfx);
}

void ResourceManager::BindBuffers(Graphics& gfx, const string& entityID) const
{
	for (auto& buffer : registry.find(entityID)->second.buffers)
		buffer->Bind(gfx);
}

// Buffer creation
template<typename V>
UINT ResourceManager::CreateVertexBuffer(Graphics& gfx, const string& entityID, const vector<V>& vertices, UINT slot)
{
	Buffer* buffer = new VertexBuffer<V>(gfx, vertices, slot);
	registry.find(entityID)->second.buffers.push_back(buffer);
	return registry.find(entityID)->second.buffers.size() - 1;
}

template<typename T>
UINT ResourceManager::CreateInputStructuredBuffer(Graphics& gfx, const string& entityID, const vector<T>& data, ShaderDataTypes type, UINT slot)
{
	Buffer* buffer = new InputStructuredBuffer<T>(gfx, data, slot);
	registry.find(entityID)->second.buffers.push_back(buffer);
	return registry.find(entityID)->second.buffers.size() - 1;
}

template<typename T>
UINT ResourceManager::CreateOutputStructuredBuffer(Graphics& gfx, const string& entityID, const vector<T>& data, UINT slot)
{
	Buffer* buffer = new OutputStructuredBuffer<T>(gfx, data, slot);
	registry.find(entityID)->second.buffers.push_back(buffer);
	return registry.find(entityID)->second.buffers.size() - 1;
}

template<typename T>
UINT ResourceManager::CreateDynamicConstantBuffer(Graphics& gfx, const string& entityID, const T& cbData, ShaderDataTypes type, UINT slot)
{
	Buffer* buffer = new DynamicConstantBuffer<T>(gfx, cbData, slot);
	registry.find(entityID)->second.buffers.push_back(buffer);
	return registry.find(entityID)->second.buffers.size() - 1;
}

template<typename T>
UINT ResourceManager::CreateStaticConstantBuffer(Graphics& gfx, const string& entityID, const T& data, UINT slot)
{
	Buffer* buffer = new StaticConstantBuffer<T>(gfx, cbData, slot);
	registry.find(entityID)->second.buffers.push_back(buffer);
	return registry.find(entityID)->second.buffers.size() - 1;
}

UINT ResourceManager::CreateIndexBuffer(Graphics& gfx, const string& entityID, const vector<unsigned short>& indices)
{
	Buffer* buffer = new IndexBuffer(gfx, indices);
	registry.find(entityID)->second.buffers.push_back(buffer);
	return registry.find(entityID)->second.buffers.size() - 1;
}

// Shader creation
void ResourceManager::CreateVertexShader(Graphics& gfx, const string& entityID, const string& path, const vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc) {
	Shader* shader = new VertexShader(gfx, path, layoutDesc);
	registry.find(entityID)->second.shaders.push_back(shader);
}

void ResourceManager::CreatePixelShader(Graphics& gfx, const string& entityID, const string& path) {
	Shader* shader = new PixelShader(gfx, path);
	registry.find(entityID)->second.shaders.push_back(shader);
}

void ResourceManager::CreateComputeShader(Graphics& gfx, const string& entityID, const string& path) {
	Shader* shader = new ComputeShader(gfx, path);
	registry.find(entityID)->second.shaders.push_back(shader);
}

//void ResourceManager::SpawnControlWindow() {
//	ImGui::Begin("Resource Manager");
//	for (auto& entity : entities) {
//		if (ImGui::TreeNode(entity.first.c_str())) {
//			if (ImGui::TreeNode("Static resources")) {
//				for (auto& resource : entity.second.staticResources) {
//					ImGui::Text("SR");
//				}
//				ImGui::TreePop();
//			}
//			if (ImGui::TreeNode("Instances")) {
//				for (int i = 0; i < entity.second.instances.size(); i++) {
//					if (ImGui::TreeNode(to_string(i).c_str())) {
//						for (auto& resource : entity.second.instances[i].resources) {
//							ImGui::Text("IR");
//						}
//						ImGui::TreePop();
//					}
//				}
//				ImGui::TreePop();
//			}
//			ImGui::TreePop();
//		}
//	}
//	ImGui::End();
//}

//void ResourceManager::DrawInstances(Graphics& gfx, const string& entityID)
//{
//	UINT indexCount = reinterpret_cast<IndexBuffer*>(GetStaticResourceByIndex(entityID, 1))->GetCount();
//	UINT instanceCount = GetInstanceCount(entityID);
//	gfx.DrawInstanced(indexCount, instanceCount);
//};

//void ResourceManager::Dispatch(Graphics& gfx, string entityID, UINT threadGroupsX, UINT threadGroupsY, UINT threadGroupsZ) const {
//	GraphicsResource* resource = entities.find(entityID)->second.staticResources[0];
//	reinterpret_cast<ComputeShader*>(resource)->Execute(gfx, threadGroupsX, threadGroupsY, threadGroupsZ);
//}