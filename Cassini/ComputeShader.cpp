#include "ComputeShader.h"

ComputeShader::ComputeShader(Graphics& gfx, string path)
{
	INFOMAN(gfx);
	LPCWSTR converted = utility::ConvertToLPWSTR(path);
	D3DReadFileToBlob(converted, &pBlob);
	GFX_THROW_INFO(gfx.GetDevice()->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader));
};

void ComputeShader::Bind(Graphics& gfx) const
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(gfx.GetContext()->CSSetShader(pComputeShader.Get(), nullptr, 0u));
}

void ComputeShader::Execute(Graphics& gfx, UINT threadGroupsX, UINT threadGroupsY, UINT threadGroupsZ) {
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(gfx.GetContext()->Dispatch(threadGroupsX, threadGroupsY, threadGroupsZ));
}