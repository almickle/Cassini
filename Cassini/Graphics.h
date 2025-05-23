#pragma once
#include "ChiliException.h"
#include "DataTypes.h"
#include "dxerr.h"
#include "DxgiInfoManager.h"
#include "GraphicsThrowMacros.h"
#include "imgui.h"
#include "InstanceData.h"
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
		IDXGISwapChain* g_pSwapChain,
		ID3D11DeviceContext* g_pd3dDeferredContext);
	void DrawIndexed(UINT count);
	void DrawInstancedIndexed(UINT indexCount, UINT instanceCount);
	void Dispatch(UINT threadGroupsX, UINT threadGroupsY, UINT threadGroupsZ);
public:
	void CreateSceneTexture();
	void CreatDepthBuffer();
	void ClearBuffer();
public:
	void SetProjection(XMMATRIX proj);
	void SetCameraTransform(XMMATRIX view);
	void SetLighting(PhongLightingData cbData);
public:
	XMMATRIX GetProjection() const;
	XMMATRIX GetCameraView() const;
	ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const;
	PhongLightingData GetLighting() const;
	ImTextureID GetSceneTexture();
	Resolution GetResolution() const;
public:
	ComPtr<ID3D11Device> GetDevice() { return pDevice; };
	ComPtr<ID3D11DeviceContext> GetContext() { return pContext; };
private:
	void FetchResolution();
private:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11DeviceContext> pDeferredContext;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11DepthStencilView> pDSV;
	ComPtr<ID3D11RenderTargetView> pSceneTarget;
	ComPtr<ID3D11ShaderResourceView> pSceneTexture;
private:
	XMMATRIX projection = XMMatrixIdentity();
	XMMATRIX cameraView = XMMatrixIdentity();
	XMMATRIX cameraTransform = XMMatrixIdentity();
	PhongLightingData lightData;
	const float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	Resolution resolution = { 0, 0 };
public:
	DxgiInfoManager infoManager;
	DxgiInfoManager& GetInfoManager() { return infoManager; }
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
};