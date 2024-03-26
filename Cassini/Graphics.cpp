#include "Graphics.h"

void
Graphics::DrawIndexed(UINT count)
{
  GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}

void
Graphics::SetProjection(FXMMATRIX proj)
{
  projection = proj;
}

XMMATRIX
Graphics::GetProjection()
{
  return projection;
}

void
Graphics::CreateSceneTexture()
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
  textureDesc.Width = textureSize;
  textureDesc.Height = textureSize;
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
  GFX_THROW_INFO(pDevice->CreateRenderTargetView(
    renderTargetTextureMap, &renderTargetViewDesc, pTarget.GetAddressOf()));

  // renderTargetTextureMap->Release();

  ID3D11ShaderResourceView* shaderResourceViewMap;
  D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

  // Setup the description of the shader resource view.
  shaderResourceViewDesc.Format = textureDesc.Format;
  shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
  shaderResourceViewDesc.Texture2D.MipLevels = 1;

  // Create the shader resource view.
  GFX_THROW_INFO(pDevice->CreateShaderResourceView(
    renderTargetTextureMap, &shaderResourceViewDesc, &shaderResourceViewMap));

  // Bind render target
  pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pDSV.Get());

  pContext->PSSetShaderResources(
    0, 1, &shaderResourceViewMap); // Draw the map to the square

  D3D11_VIEWPORT viewport;
  viewport.Width = textureSize;
  viewport.Height = textureSize;
  viewport.MinDepth = 0;
  viewport.MaxDepth = 1;
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  pContext->RSSetViewports(1, &viewport);

  sceneTexture = shaderResourceViewMap;
}

void
Graphics::ClearBuffer()
{
  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  pContext->ClearRenderTargetView(pTarget.Get(), clearColor);
  pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

ImTextureID
Graphics::GetSceneTexture()
{
  return sceneTexture;
}

void
Graphics::RenderScene(ImVec2 size)
{
  struct Vertex
  {
    struct
    {
      float x;
      float y;
      float z;
    } pos;
  };

  // Create vertex buffer
  Vertex vertices[] = {
    { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f },
    { 1.0f, 1.0f, -1.0f },   { -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f },   { 1.0f, 1.0f, 1.0f },
  };
  ComPtr<ID3D11Buffer> pVertexBuffer;
  D3D11_BUFFER_DESC bd = {};
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.CPUAccessFlags = 0u;
  bd.MiscFlags = 0u;
  bd.ByteWidth = sizeof(vertices);
  bd.StructureByteStride = sizeof(Vertex);
  D3D11_SUBRESOURCE_DATA sd = {};
  sd.pSysMem = vertices;
  (pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

  // Bind vertex buffer to pipeline
  const UINT stride = sizeof(Vertex);
  const UINT offset = 0u;
  pContext->IASetVertexBuffers(
    0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

  // Create index buffer
  const unsigned short indices[] = { 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5,
                                     2, 6, 3, 3, 6, 7, 4, 5, 7, 4, 7, 6,
                                     0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 };
  ComPtr<ID3D11Buffer> pIndexBuffer;
  D3D11_BUFFER_DESC ibd = {};
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.Usage = D3D11_USAGE_DEFAULT;
  ibd.CPUAccessFlags = 0u;
  ibd.MiscFlags = 0u;
  ibd.ByteWidth = sizeof(indices);
  ibd.StructureByteStride = sizeof(unsigned short);
  D3D11_SUBRESOURCE_DATA isd = {};
  isd.pSysMem = indices;
  (pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

  // Bind index buffer
  pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

  // Create constant buffer for transformation matrix
  struct ConstantBuffer
  {
    XMMATRIX transform;
  };
  float angle = sin(frameCount * 3.14 / 180);
  float angle2 = cos(frameCount * 3.14 / 180);
  const ConstantBuffer cb = { { XMMatrixTranspose(
    XMMatrixRotationZ(angle) * XMMatrixRotationX(angle2) *
    XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
    XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 10.0f)) } };
  ComPtr<ID3D11Buffer> pConstantBuffer;
  D3D11_BUFFER_DESC cbd;
  cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbd.Usage = D3D11_USAGE_DYNAMIC;
  cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  cbd.MiscFlags = 0u;
  cbd.ByteWidth = sizeof(cb);
  cbd.StructureByteStride = 0u;
  D3D11_SUBRESOURCE_DATA csd = {};
  csd.pSysMem = &cb;
  (pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

  // Bind constant buffer to vertex shader
  pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

  // Lookup table for cube face colors
  struct ConstantBuffer2
  {
    struct
    {
      float r;
      float g;
      float b;
      float a;
    } face_colors[6];
  };
  const ConstantBuffer2 cb2 = { {
    { 1.0f, 0.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f, 1.0f },
  } };
  ComPtr<ID3D11Buffer> pConstantBuffer2;
  D3D11_BUFFER_DESC cbd2;
  cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbd2.Usage = D3D11_USAGE_DEFAULT;
  cbd2.CPUAccessFlags = 0u;
  cbd2.MiscFlags = 0u;
  cbd2.ByteWidth = sizeof(cb2);
  cbd2.StructureByteStride = 0u;
  D3D11_SUBRESOURCE_DATA csd2 = {};
  csd2.pSysMem = &cb2;
  (pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

  // Bind constant buffer to pixel shader
  pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

  // Create pixel shader
  ComPtr<ID3D11PixelShader> pPixelShader;
  ComPtr<ID3DBlob> pBlob;
  (D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
  (pDevice->CreatePixelShader(
    pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

  // Bind pixel shader
  pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

  // Create vertex shader
  ComPtr<ID3D11VertexShader> pVertexShader;
  (D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
  (pDevice->CreateVertexShader(pBlob->GetBufferPointer(),
                               pBlob->GetBufferSize(),
                               nullptr,
                               &pVertexShader));

  // Bind vertex shader
  pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

  // Input vertex layout
  ComPtr<ID3D11InputLayout> pInputLayout;
  const D3D11_INPUT_ELEMENT_DESC ied[] = {
    { "Position",
      0,
      DXGI_FORMAT_R32G32B32_FLOAT,
      0,
      0,
      D3D11_INPUT_PER_VERTEX_DATA,
      0 },
  };
  (pDevice->CreateInputLayout(ied,
                              (UINT)std::size(ied),
                              pBlob->GetBufferPointer(),
                              pBlob->GetBufferSize(),
                              &pInputLayout));

  // Bind vertex layout
  pContext->IASetInputLayout(pInputLayout.Get());

  // Set primitive topology to triangle list (groups of 3 vertices)
  pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);

  frameCount++;
}

void
Graphics::CreatDepthBuffer()
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
  descDepth.Width = textureSize;
  descDepth.Height = textureSize;
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