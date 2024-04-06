#pragma once
#include "Graphics.h"
#include "Particle.h"
#include "ResourceManager.h"
#include "Utility.h"

class ElectricField
{
public:
	//ElectricField(Graphics& gfx, ResourceManager& manager, vector<Particle*> ptcls)
	//	:particles(ptcls)
	//{
	//	entityID = utility::GenerateUniqueID();
	//	instanceID = utility::GenerateUniqueID();
	//	manager.RegisterEntity(entityID);
	//	manager.RegisterInstance(entityID, instanceID);
	//	manager.CreateComputeShader(gfx, entityID, "FieldCS.cso");
	//	const UINT res[3] = { 100, 100, 100 };
	//	textureSRV = manager.CreateTexture3D(gfx, entityID, res);
	//	resourceID = utility::GenerateUniqueID();
	//	//FieldBuffer fieldBuffer = {};
	//	//for (int i = 0; i < particles.size(); i++) {
	//	//	XMFLOAT3 pos = particles[i]->GetPosition();
	//	//	fieldBuffer.particles[i] = { pos, particles[i]->GetCharge() };
	//	//}
	//	//fieldBuffer.size = particles.size();
	//	manager.CreateConstantBuffer(gfx, entityID, instanceID, resourceID, COMPUTE_SHADER_BUFFER, fieldBuffer);
	//}

	ElectricField(Graphics& gfx, ResourceManager& manager, vector<Particle*> ptcls)
		:particles(ptcls)
	{
		entityID = utility::GenerateUniqueID();
		instanceID = utility::GenerateUniqueID();
		manager.RegisterEntity(entityID);
		manager.RegisterInstance(entityID, instanceID);
		manager.CreateComputeShader(gfx, entityID, "ParticleCS.cso");
		vector<ParticleData> particleInput = {
			{
				{ 0.0f, 0.0f, 0.0f },
			{ 10.0f, 10.0f, 10.0f },
			}
		};
		inputBuffer = manager.CreateStructuredBuffer(gfx, entityID, particleInput, 0, true);
		vector<ParticleData> particleOutput = {
			{
				{ 20.0f, 20.0f, 20.0f },
			{ 30.0f, 30.0f, 30.0f },
			}
		};
		outputBuffer = manager.CreateStructuredBuffer(gfx, entityID, particleOutput, 0, false);
	}

	void SpawnControlWindow() {
		ImGui::Begin("Field Texture");
		ImGui::SliderInt("3D slice", &depthSlice, 0, 100);
		ImGui::Image(sliceSRV.Get(), ImVec2(200, 200));
		ImGui::End();
	}

	void GetTextureSlice(Graphics& gfx, int index) {
		//D3D11_TEXTURE3D_DESC texDesc;
		////pTexture3D->GetDesc(&texDesc);

		//// Create a 2D texture for the slice
		//ID3D11Texture2D* pSliceTexture = nullptr;
		//D3D11_TEXTURE2D_DESC sliceTexDesc;

		//sliceTexDesc.Width = texDesc.Width;
		//sliceTexDesc.Height = texDesc.Height;
		//sliceTexDesc.MipLevels = 1;
		//sliceTexDesc.ArraySize = 1;
		//sliceTexDesc.Format = texDesc.Format;
		//sliceTexDesc.SampleDesc.Count = 1;
		//sliceTexDesc.SampleDesc.Quality = 0;
		//sliceTexDesc.Usage = D3D11_USAGE_DEFAULT;
		//sliceTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		//sliceTexDesc.CPUAccessFlags = 0;
		//sliceTexDesc.MiscFlags = 0;

		// Create the 2D texture
		//HRESULT hr = pDevice->CreateTexture2D(&sliceTexDesc, nullptr, &pSliceTexture);
		gfx.GetContext();
	}

	void AddChargeContributor(Particle* atom) {
		particles.push_back(atom);
	}

	void Bind(Graphics& gfx, ResourceManager& manager) {
		manager.BindStaticResources(gfx, entityID);
		manager.BindInstanceResources(gfx, entityID, instanceID);
	}
	void Dispatch(Graphics& gfx, ResourceManager& manager, UINT threadCount)
	{
		manager.Dispatch(gfx, entityID, threadCount, 1, 1);
		StructuredBuffer* output = reinterpret_cast<StructuredBuffer*>(manager.GetStaticResourceByIndex(entityID, outputBuffer));
		vector<ParticleData> data = output->ReadFromBuffer(gfx);
		StructuredBuffer* input = reinterpret_cast<StructuredBuffer*>(manager.GetStaticResourceByIndex(entityID, inputBuffer));
		input->WriteToBuffer(gfx, data);
	};
private:
	GraphicsResource* GetResource(ResourceManager& manager, int index) {
		return manager.GetStaticResourceByIndex(entityID, index);
	}
private:
	string entityID;
	string instanceID;
	string resourceID;
private:
	int inputBuffer;
	int outputBuffer;
private:
	vector<Particle*> particles;
	ComPtr<ID3D11ShaderResourceView> textureSRV;
	ComPtr<ID3D11Texture2D> pSlice;
	ComPtr<ID3D11ShaderResourceView> sliceSRV;
	int depthSlice = 0;
};

