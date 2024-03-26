#pragma once
#include "ChiliException.h"
#include "dxerr.h"
#include "DxgiInfoManager.h"
#include "GraphicsThrowMacros.h"
#include "imgui.h"
#include <cmath>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include <sstream>
#include <vector>
#include <wrl/client.h>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

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

    infoManager.Set();

    CreatDepthBuffer();
    CreateSceneTexture();
  }

  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  ~Graphics() = default;

  friend class Bindable;

  void DrawIndexed(UINT count);
  void SetProjection(FXMMATRIX proj);
  XMMATRIX GetProjection();

  void CreateSceneTexture();
  void RenderScene(ImVec2 size);
  void CreatDepthBuffer();
  void ClearBuffer();
  ImTextureID GetSceneTexture();

private:
  ComPtr<ID3D11Device> pDevice;
  ComPtr<ID3D11DeviceContext> pContext;
  ComPtr<IDXGISwapChain> pSwap;
  ComPtr<ID3D11DepthStencilView> pDSV;
  ComPtr<ID3D11RenderTargetView> pTarget;

  ID3D11ShaderResourceView* sceneTexture;

private:
  XMMATRIX projection;
  float textureSize = 1000.0f;
  int frameCount = 0;

public:
  class Exception : public ChiliException
  {
    using ChiliException::ChiliException;
  };
  class HrException : public Exception
  {
  public:
    HrException(int line,
                const char* file,
                HRESULT hr,
                std::vector<std::string> infoMsgs = {}) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    HRESULT GetErrorCode() const noexcept;
    std::string GetErrorString() const noexcept;
    std::string GetErrorDescription() const noexcept;
    std::string GetErrorInfo() const noexcept;

  private:
    HRESULT hr;
    std::string info;
  };
  class InfoException : public Exception
  {
  public:
    InfoException(int line,
                  const char* file,
                  std::vector<std::string> infoMsgs) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    std::string GetErrorInfo() const noexcept;

  private:
    std::string info;
  };
  class DeviceRemovedException : public HrException
  {
    using HrException::HrException;

  public:
    const char* GetType() const noexcept override;

  private:
    std::string reason;
  };
  DxgiInfoManager infoManager;
};