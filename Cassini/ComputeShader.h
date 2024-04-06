#pragma once
#include "GraphicsResource.h"
#include "Utility.h"

class ComputeShader : public GraphicsResource
{
public:
	ComputeShader(Graphics& gfx, string path)
	{
		INFOMAN(gfx);
		LPCWSTR converted = utility::ConvertToLPWSTR(path);
		D3DReadFileToBlob(converted, &pBlob);
		GFX_THROW_INFO(gfx.GetDevice()->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader));
	};

	void Bind(Graphics& gfx) const override
	{
		INFOMAN(gfx);
		GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetShader(pComputeShader.Get(), nullptr, 0u));
	}

	void Execute(Graphics& gfx, UINT threadGroupsX, UINT threadGroupsY, UINT threadGroupsZ) {
		INFOMAN(gfx);
		GFX_THROW_INFO_ONLY(gfx.GetContext()->Dispatch(threadGroupsX, threadGroupsY, threadGroupsZ));
	}

	ComPtr<ID3DBlob> GetBlob() { return pBlob; }


private:
	ComPtr<ID3D11ComputeShader> pComputeShader;
	ComPtr<ID3DBlob> pBlob;
};