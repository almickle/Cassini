#pragma once
#include "imgui.h"
#include <cmath>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl/client.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class Graphics
{
public:
  Graphics(ID3D11Device* g_pd3dDevice,
           ID3D11DeviceContext* g_pd3dDeviceContext,
           IDXGISwapChain* g_pSwapChain)
  {
    pDevice = g_pd3dDevice;
    pContext = g_pd3dDeviceContext;
    pSwap = g_pSwapChain;
  }

  ImTextureID CreateSceneTexture(ImVec2 size);
  void RenderScene(ImVec2 size);
  void CreatDepthStencilView();

private:
  ComPtr<ID3D11Device> pDevice;
  ComPtr<ID3D11DeviceContext> pContext;
  ComPtr<IDXGISwapChain> pSwap;
  ComPtr<ID3D11DepthStencilView> pDSV;

  float textureSize = 1000.0f;

  int frameCount = 0;
};
