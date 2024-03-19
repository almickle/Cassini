#pragma once
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
		IDXGISwapChain* g_pSwapChain, 
		ID3D11RenderTargetView* g_mainRenderTargetView) 
	{
		pDevice = g_pd3dDevice;
		pContext = g_pd3dDeviceContext;
		pSwap = g_pSwapChain;
		pTarget = g_mainRenderTargetView;
	}
	void DrawTriangle() {
		struct Vertex {
			float x;
			float y;
		};
		const Vertex vertices[] = {
			{0.0f, 0.5f},
			{0.5f, -0.5f},
			{-0.5f, -0.5f}
		};
		ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bufferDescriptor = {};
		bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
		bufferDescriptor.CPUAccessFlags = 0u;
		bufferDescriptor.MiscFlags = 0u;
		bufferDescriptor.ByteWidth = sizeof(vertices);
		bufferDescriptor.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices;
		pDevice->CreateBuffer(&bufferDescriptor, &subresourceData, &pVertexBuffer);

		// Bind vertex buffer to pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		// Create pixel shader
		ID3DBlob* pBlob;
		ID3D11PixelShader* pPixelShader;
		D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
		pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
		
		// Bind pixel shader
		pContext->PSSetShader(pPixelShader, nullptr, 0u);

		// Create vertex shader
		ID3D11VertexShader* pVertexShader;
		D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
		pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
		
		// Bind vertex shader
		pContext->VSSetShader(pVertexShader, nullptr, 0u);

		// Create input layout
		ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

		// Bind input layout
		pContext->IASetInputLayout( pInputLayout.Get() );

		// Bind render target
		pContext->OMSetRenderTargets( 1, pTarget.GetAddressOf(), nullptr );

		// Set primitive topology
		pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		D3D11_VIEWPORT viewport;
		viewport.Width = 800;
		viewport.Height = 600;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		pContext->RSSetViewports(1, &viewport);

		pContext->Draw((UINT)std::size(vertices), 0u);
	}

private:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11RenderTargetView> pTarget;
};

