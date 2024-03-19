#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

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
		//ID3D11Buffer* pVertexBuffer;
		//D3D11_BUFFER_DESC bufferDescriptor = {};
		//bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
		//bufferDescriptor.CPUAccessFlags = 0u;
		//bufferDescriptor.MiscFlags = 0u;
		//bufferDescriptor.ByteWidth = sizeof(vertices);
		//bufferDescriptor.StructureByteStride = sizeof(Vertex);
		//D3D11_SUBRESOURCE_DATA subresourceData = {};
		//subresourceData.pSysMem = vertices;
		//pDevice->CreateBuffer(&bufferDescriptor, &subresourceData, &pVertexBuffer);
		//const UINT stride = sizeof(Vertex);
		//const UINT offset = 0u;
		//pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

		//ID3D11VertexShader* pVertexShader;
		//ID3DBlob* pBlob;
		//D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
		//pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

		//pContext->VSSetShader(pVertexShader, nullptr, 0u);
		pContext->Draw(sizeof(vertices), 0u);
	}

private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	IDXGISwapChain* pSwap;
	ID3D11RenderTargetView* pTarget;
};

