#pragma once
#include "imgui.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class Graphics
{
public:
	Graphics(
		ID3D11Device* g_pd3dDevice,
		ID3D11DeviceContext* g_pd3dDeviceContext,
		IDXGISwapChain* g_pSwapChain
	)
	{
		pDevice = g_pd3dDevice;
		pContext = g_pd3dDeviceContext;
		pSwap = g_pSwapChain;
	}
	ImTextureID CreateSceneTexture(ImVec2 size);

private:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<IDXGISwapChain> pSwap;
};

