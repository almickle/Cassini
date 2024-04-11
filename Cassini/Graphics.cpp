#include "Graphics.h"

Graphics::Graphics(ID3D11Device* g_pd3dDevice,
	ID3D11DeviceContext* g_pd3dDeviceContext,
	IDXGISwapChain* g_pSwapChain,
	ID3D11DeviceContext* g_pd3dDeferredContext)
{
	pDevice = g_pd3dDevice;
	pContext = g_pd3dDeviceContext;
	pSwap = g_pSwapChain;
	pDeferredContext = g_pd3dDeferredContext;

	infoManager.Set();

	FetchResolution();

	CreatDepthBuffer();
	CreateSceneTexture();
}

void Graphics::DrawIndexed(UINT count)
{
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}

void Graphics::DrawInstancedIndexed(UINT indexCount, UINT instanceCount)
{
	GFX_THROW_INFO_ONLY(pContext->DrawIndexedInstanced(indexCount, instanceCount, 0u, 0u, 0u));
}

void Graphics::Dispatch(UINT threadGroupsX, UINT threadGroupsY, UINT threadGroupsZ)
{
	GFX_THROW_INFO_ONLY(pContext->Dispatch(threadGroupsX, threadGroupsY, threadGroupsZ));
}

XMMATRIX Graphics::GetProjection() const
{
	return XMMatrixTranspose(projection);
}

void Graphics::SetProjection(XMMATRIX proj)
{
	projection = proj;
}

void Graphics::SetLighting(PhongLightingData cbData) {
	lightData = cbData;
}
PhongLightingData Graphics::GetLighting() const {
	return lightData;
}

XMMATRIX Graphics::GetCameraView() const
{
	return XMMatrixTranspose(XMMatrixInverse(nullptr, cameraTransform));
}

void Graphics::SetCameraTransform(XMMATRIX tansform)
{
	cameraTransform = tansform;
}

void Graphics::CreateSceneTexture()
{
	HRESULT hr;
	////// Render scene to texture ///////

	D3D11_TEXTURE2D_DESC textureDesc;
	ID3D11Texture2D* renderTargetTextureMap;

	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the texture description.
	// We will need to have this texture bound as a render target AND a shader
	// resource
	textureDesc.Width = resolution.width;
	textureDesc.Height = resolution.height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	GFX_THROW_INFO(
		pDevice->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(renderTargetTextureMap,
		&renderTargetViewDesc,
		pSceneTarget.GetAddressOf()));

	renderTargetTextureMap->Release();

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	GFX_THROW_INFO(
		pDevice->CreateShaderResourceView(renderTargetTextureMap,
		&shaderResourceViewDesc,
		pSceneTexture.GetAddressOf()));

	pContext->PSSetShaderResources(
		0, 1, pSceneTexture.GetAddressOf()); // Draw the map to the square

	D3D11_VIEWPORT viewport;
	viewport.Width = resolution.width;
	viewport.Height = resolution.height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	pContext->RSSetViewports(1, &viewport);
}

void Graphics::FetchResolution()
{
	HRESULT hr;
	IDXGIFactory* factory;
	GFX_THROW_INFO(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));

	IDXGIAdapter* adapter;
	GFX_THROW_INFO(factory->EnumAdapters(0, &adapter));

	IDXGIOutput* output;
	GFX_THROW_INFO(adapter->EnumOutputs(0, &output));

	DXGI_OUTPUT_DESC desc;
	GFX_THROW_INFO(output->GetDesc(&desc));

	resolution.width =
		desc.DesktopCoordinates.right - desc.DesktopCoordinates.left;
	resolution.height =
		desc.DesktopCoordinates.bottom - desc.DesktopCoordinates.top;

	output->Release();
	adapter->Release();
	factory->Release();
}

Resolution Graphics::GetResolution() const
{
	return resolution;
}

void Graphics::ClearBuffer()
{
	// Bind render target
	pContext->OMSetRenderTargets(1, pSceneTarget.GetAddressOf(), pDSV.Get());
	pContext->ClearRenderTargetView(pSceneTarget.Get(), clearColor);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

ImTextureID Graphics::GetSceneTexture()
{
	return pSceneTexture.Get();
}

void Graphics::CreatDepthBuffer()
{
	HRESULT hr;

	// create depth stensil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stensil texture
	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = resolution.width;
	descDepth.Height = resolution.height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(
		pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));
}

// Graphics exception stuff
Graphics::HrException::HrException(int line,
	const char* file,
	HRESULT hr,
	std::vector<std::string> infoMsgs) noexcept
	: Exception(line, file)
	, hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty()) {
		info.pop_back();
	}
}

const char*
Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty()) {
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char*
Graphics::HrException::GetType() const noexcept
{
	return "Chili Graphics Exception";
}

HRESULT
Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string
Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorStringA(hr);
}

std::string
Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescriptionA(hr, buf, sizeof(buf));
	return buf;
}

std::string
Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char*
Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Chili Graphics Exception [Device Removed] "
		"(DXGI_ERROR_DEVICE_REMOVED)";
}
Graphics::InfoException::InfoException(
	int line,
	const char* file,
	std::vector<std::string> infoMsgs) noexcept
	: Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty()) {
		info.pop_back();
	}
}

const char*
Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n"
		<< GetErrorInfo() << std::endl
		<< std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char*
Graphics::InfoException::GetType() const noexcept
{
	return "Chili Graphics Info Exception";
}

std::string
Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}