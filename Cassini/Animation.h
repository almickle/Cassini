#pragma once
#include "Camera.h"
#include "ParticleSystem.h"
#include "PointLight.h"
#include <chrono>

class Animation
{
public:
	Animation(Graphics& gfx, ResourceManager& manager, ParticleSystem* in_system, Camera* in_camera, PointLight* in_light)
		: system(in_system), camera(in_camera), light(in_light)
	{
		UINT frames = simulationTime * frameRate;
		frameBuffer.reserve(frames);
		for (int i = 0; i < frames; i++)
		{
			ID3D11Texture2D* pFrameTexture;
			ComPtr<ID3D11RenderTargetView> pFrameRenderTarget;
			ComPtr<ID3D11ShaderResourceView> pFrameSRV;
			frameBuffer.push_back(pFrameSRV.Get());
			textureBuffer.push_back(pFrameTexture);
			renderTargetBuffer.push_back(pFrameRenderTarget);
		}
	}

	void Step(Graphics& gfx, ResourceManager& manager, Resolution resolution, bool draw)
	{
		UINT width = gfx.GetResolution().width;
		UINT height = gfx.GetResolution().height;
		gfx.SetProjection(XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 10000.0f));
		camera->UpdateCamera(gfx);
		//camera->SetTarget(system->GetBond(0)->GetMove());
		camera->SetTarget(system->GetCentroid());
		light->UpdateLight(gfx);
		manager.BindGlobalBuffers(gfx);
		for (auto& system : manager.GetSystemList())
		{
			manager.BindResources(gfx, system.first, SYSTEM);
			manager.GetSystemInstances(system.first)[0]->Update(gfx, manager, stepTime);
			for (auto& entity : manager.GetSystemEntityList(system.first))
			{
				manager.BindResources(gfx, system.first, entity.first);
				if (draw)
					manager.DrawSystemEntityInstances(gfx, system.first, entity.first);
			}
		}
		for (auto& entity : manager.GetEntityList())
		{
			manager.BindResources(gfx, entity.first, ENTITY);
			manager.UpdateGlobalBuffer(gfx, 0u, ViewProjection{ gfx.GetCameraView(), gfx.GetProjection() });
			manager.UpdateGlobalBuffer(gfx, 1u, PhongLightingData{ gfx.GetLighting() });
			for (auto& instance : manager.GetInstances(entity.first))
			{
				instance->Update(gfx, manager);
			}
			if (draw)
				manager.DrawEntityInstances(gfx, entity.first);
		}
	}

	void Bake(Graphics& gfx, ResourceManager& manager, Resolution resolution) {
		UINT frameCount = 0u;
		bool draw = false;
		while (currentTime < simulationTime)
		{
			if (frameCount < currentTime * frameRate)
			{
				SetupFrame(gfx, manager, frameCount);
				frameCount++;
				draw = true;
			}
			Step(gfx, manager, resolution, draw);
			if (draw)
			{
				snapshots.push_back(system->TakeSnapShot(gfx, manager));
			}
			currentTime += stepTime;
			draw = false;
		}
	}

	void Watch(Graphics& gfx, ImVec2 size) {
		ImGui::SliderInt("Frame", &playbackFrame, 0, frameBuffer.size() - 1);
		if (ImGui::ArrowButton("Play", ImGuiDir_Right))
		{
			isPlaying = true;
			playbackStart = chrono::steady_clock::now();
		}
		if (isPlaying)
		{
			auto now = chrono::steady_clock::now();
			auto elapsed = chrono::duration<float>(now - playbackStart).count();
			if (elapsed > playbackFrame / frameRate && elapsed < simulationTime)
			{
				if (playbackFrame + 1 < frameBuffer.size())
					IncrementPlaybackFrame();
			}
		}
		if (ImGui::Button("Pause"))
		{
			isPlaying = false;
		}
	}

private:
	void SetupFrame(Graphics& gfx, ResourceManager& manager, UINT frame)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = gfx.GetResolution().width;
		textureDesc.Height = gfx.GetResolution().height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		gfx.GetDevice()->CreateTexture2D(&textureDesc, NULL, &textureBuffer[frame]);

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		gfx.GetDevice()->CreateRenderTargetView(textureBuffer[frame], &renderTargetViewDesc, renderTargetBuffer[frame].GetAddressOf());

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		gfx.GetDevice()->CreateShaderResourceView(textureBuffer[frame], &shaderResourceViewDesc, &frameBuffer[frame]);

		D3D11_VIEWPORT viewport;
		viewport.Width = gfx.GetResolution().width;
		viewport.Height = gfx.GetResolution().height;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		gfx.GetContext()->RSSetViewports(1, &viewport);

		const float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		gfx.GetContext()->OMSetRenderTargets(1u, renderTargetBuffer[frame].GetAddressOf(), gfx.GetDepthStencilView().Get());
		gfx.GetContext()->ClearRenderTargetView(renderTargetBuffer[frame].Get(), clearColor);
		gfx.GetContext()->ClearDepthStencilView(gfx.GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

public:
	UINT GetCurrentFrame() const
	{
		return playbackFrame;
	}

	vector<ParticleData> GetSnapshot() const
	{
		return snapshots[playbackFrame];
	}
	void ShowFrameSlider()
	{
		ImGui::SliderInt("Frame", &playbackFrame, 0, frameBuffer.size() - 1);
	}
	void SetPlaybackStart(chrono::steady_clock::time_point time)
	{
		playbackStart = time;
	}
	chrono::steady_clock::time_point GetPlaybackStart() const
	{
		return playbackStart;
	}
	float GetFrameRate() const
	{
		return frameRate;
	}
	float GetPlaybackFrame() const
	{
		return playbackFrame;
	}
	void IncrementPlaybackFrame()
	{
		playbackFrame++;
	}
private:
	vector<ID3D11Texture2D*> textureBuffer;
	vector<ID3D11ShaderResourceView*> frameBuffer;
	vector<ComPtr<ID3D11RenderTargetView>> renderTargetBuffer;
private:
	vector<vector<ParticleData>> snapshots;
private:
	float stepTime = 0.01f; // picoseconds
	float simulationTime = 60.0f; // picoseconds
	float frameRate = 60.0f; // f / ps
	float currentTime = 0.0f; // seconds
private:
	bool isPlaying = false;
	chrono::steady_clock::time_point playbackStart;
	float playbackCurrent = 0.0f;
	int playbackFrame = 0u;
private:
	PointLight* light;
	Camera* camera;
	ParticleSystem* system;
};