#include "Graphics.h"

ImTextureID Graphics::CreateSceneTexture(ImVec2 size) {
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
	pContext->IASetInputLayout(pInputLayout.Get());

	// Set primitive topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Render scene to texture

	D3D11_TEXTURE2D_DESC textureDesc;
	ID3D11Texture2D* renderTargetTextureMap;

	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the texture description.
	// We will need to have this texture bound as a render target AND a shader resource
	textureDesc.Width = size.x;
	textureDesc.Height = size.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	pDevice->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap);

	ID3D11RenderTargetView* renderTargetViewMap;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	pDevice->CreateRenderTargetView(renderTargetTextureMap, &renderTargetViewDesc, &renderTargetViewMap);

	ID3D11ShaderResourceView* shaderResourceViewMap;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	pDevice->CreateShaderResourceView(renderTargetTextureMap, &shaderResourceViewDesc, &shaderResourceViewMap);

	// Bind render target
	pContext->OMSetRenderTargets(1, &renderTargetViewMap, nullptr);

	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	pContext->ClearRenderTargetView(renderTargetViewMap, clear_color_with_alpha);

	pContext->PSSetShaderResources(0, 1, &shaderResourceViewMap);    // Draw the map to the square

	D3D11_VIEWPORT viewport;
	viewport.Width = size.x;
	viewport.Height = size.y;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	pContext->RSSetViewports(1, &viewport);

	pContext->Draw((UINT)std::size(vertices), 0u);

	return shaderResourceViewMap;
}