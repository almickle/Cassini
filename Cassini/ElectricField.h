//#pragma once
//#include "Graphics.h"
//#include "Particle.h"
//#include "ResourceManager.h"
//#include "Utility.h"
//
//class ElectricField
//{
//public:
//	//ElectricField(Graphics& gfx, ResourceManager& manager, vector<Particle*> ptcls)
//	//	:particles(ptcls)
//	//{
//	//	entityID = utility::GenerateUniqueID();
//	//	instanceID = utility::GenerateUniqueID();
//	//	manager.RegisterEntity(entityID);
//	//	manager.RegisterInstance(entityID, instanceID);
//	//	manager.CreateComputeShader(gfx, entityID, "FieldCS.cso");
//	//	const UINT res[3] = { 100, 100, 100 };
//	//	textureSRV = manager.CreateTexture3D(gfx, entityID, res);
//	//	resourceID = utility::GenerateUniqueID();
//	//	//FieldBuffer fieldBuffer = {};
//	//	//for (int i = 0; i < particles.size(); i++) {
//	//	//	XMFLOAT3 pos = particles[i]->GetPosition();
//	//	//	fieldBuffer.particles[i] = { pos, particles[i]->GetCharge() };
//	//	//}
//	//	//fieldBuffer.size = particles.size();
//	//	manager.CreateConstantBuffer(gfx, entityID, instanceID, resourceID, COMPUTE_SHADER_BUFFER, fieldBuffer);
//	//}
//
//	ElectricField(Graphics& gfx, ResourceManager& manager, vector<Particle*> ptcls)
//		:particles(ptcls)
//	{
//		if (!manager.CheckForEntity(entityID)) {
//			manager.RegisterEntity(entityID);
//			manager.RegisterEntity(entityID);
//			manager.RegisterInstance(entityID);
//			manager.CreateComputeShader(gfx, entityID, "ParticleCS.cso");
//			vector<IntrinsicParticleData> intrinsicData = LoadIntrinsicData();
//			vector<ParticleData> particleData = LoadInitialParticleData();
//			SimulationData simulationData = { { -100.0f, 0.0f, -100.0f }, { 100.0f, 100.0f, 100.f }, particles.size(), 0.0f };
//			intrinsicBuffer = manager.CreateStructuredBuffer(gfx, entityID, intrinsicData, 0, true);
//			inputBuffer = manager.CreateStructuredBuffer(gfx, entityID, particleData, 1, true);
//			outputBuffer = manager.CreateStructuredBuffer(gfx, entityID, particleData, 0, false);
//			simulationBuffer = manager.CreateStaticConstantBuffer(gfx, entityID, 0u, simulationData);
//		}
//	}
//
//	vector<ParticleData> LoadInitialParticleData() {
//		vector<ParticleData> ptcls;
//		for (auto& ptcl : particles) {
//			ParticleData data = {
//				ptcl->GetPosition(),
//				ptcl->GetVelocity()
//			};
//			ptcls.push_back(data);
//		}
//		return ptcls;
//	}
//
//	vector<IntrinsicParticleData> LoadIntrinsicData() {
//		vector<IntrinsicParticleData> buffer;
//		for (auto& ptcl : particles) {
//			IntrinsicParticleData data = {
//				ptcl->GetMass(),
//				ptcl->GetCharge(),
//				ptcl->GetRadius()
//			};
//			buffer.push_back(data);
//		}
//		return buffer;
//	}
//
//	void SpawnControlWindow() {
//		ImGui::Begin("Field Texture");
//		ImGui::SliderInt("3D slice", &depthSlice, 0, 100);
//		ImGui::Image(sliceSRV.Get(), ImVec2(200, 200));
//		ImGui::End();
//	}
//
//	void GetTextureSlice(Graphics& gfx, int index) {
//		//D3D11_TEXTURE3D_DESC texDesc;
//		////pTexture3D->GetDesc(&texDesc);
//
//		//// Create a 2D texture for the slice
//		//ID3D11Texture2D* pSliceTexture = nullptr;
//		//D3D11_TEXTURE2D_DESC sliceTexDesc;
//
//		//sliceTexDesc.Width = texDesc.Width;
//		//sliceTexDesc.Height = texDesc.Height;
//		//sliceTexDesc.MipLevels = 1;
//		//sliceTexDesc.ArraySize = 1;
//		//sliceTexDesc.Format = texDesc.Format;
//		//sliceTexDesc.SampleDesc.Count = 1;
//		//sliceTexDesc.SampleDesc.Quality = 0;
//		//sliceTexDesc.Usage = D3D11_USAGE_DEFAULT;
//		//sliceTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//		//sliceTexDesc.CPUAccessFlags = 0;
//		//sliceTexDesc.MiscFlags = 0;
//
//		// Create the 2D texture
//		//HRESULT hr = pDevice->CreateTexture2D(&sliceTexDesc, nullptr, &pSliceTexture);
//		gfx.GetContext();
//	}
//
//	void AddChargeContributor(Particle* atom) {
//		particles.push_back(atom);
//	}
//
//	void Bind(Graphics& gfx, ResourceManager& manager) {
//		manager.BindStaticResources(gfx, entityID);
//		manager.BindInstanceResources(gfx, entityID, 0u);
//	}
//
//	void Dispatch(Graphics& gfx, ResourceManager& manager, UINT threadCount, float dt)
//	{
//		manager.Dispatch(gfx, entityID, threadCount, 1, 1);
//		vector<ParticleData> data = ProcessBufferOutput(gfx, manager);
//		UpdatePositionInformation(gfx, manager, data);
//		UpdateDeltaT(gfx, manager, dt);
//	};
//
//	void UpdateDeltaT(Graphics& gfx, ResourceManager& manager, float dt) {
//		StaticConstantBuffer<SimulationData>* resource = reinterpret_cast<StaticConstantBuffer<SimulationData>*>(manager.GetStaticResourceByIndex(entityID, simulationBuffer));
//		SimulationData data = { { -100.0f, 0.0f, -100.0f }, { 100.0f, 100.0f, 100.f }, particles.size(), dt };
//		resource->Update(gfx, data);
//	}
//
//	vector<ParticleData> ProcessBufferOutput(Graphics& gfx, ResourceManager& manager) {
//		StructuredBuffer<ParticleData>* output = reinterpret_cast<StructuredBuffer<ParticleData>*>(manager.GetStaticResourceByIndex(entityID, outputBuffer));
//		vector<ParticleData> data = output->ReadFromBuffer(gfx);
//		StructuredBuffer<ParticleData>* input = reinterpret_cast<StructuredBuffer<ParticleData>*>(manager.GetStaticResourceByIndex(entityID, inputBuffer));
//		input->WriteToBuffer(gfx, data);
//		return data;
//	}
//
//	void UpdatePositionInformation(Graphics& gfx, ResourceManager& manager, const vector<ParticleData>& data) {
//		for (int i = 0; i < data.size(); i++) {
//			particles[i]->SetPosition(data[i].s);
//		}
//	}
//
//	static const string entityID;
//private:
//	GraphicsResource* GetResource(ResourceManager& manager, int index) {
//		return manager.GetStaticResourceByIndex(entityID, index);
//	}
//private:
//	string instanceID;
//	string resourceID;
//private:
//	int inputBuffer;
//	int outputBuffer;
//	int intrinsicBuffer;
//	int simulationBuffer;
//private:
//	vector<Particle*> particles;
//	ComPtr<ID3D11ShaderResourceView> textureSRV;
//	ComPtr<ID3D11Texture2D> pSlice;
//	ComPtr<ID3D11ShaderResourceView> sliceSRV;
//	int depthSlice = 0;
//};
//
