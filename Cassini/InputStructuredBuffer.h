#pragma once
#include "Buffer.h"

template<typename T>
class InputStructuredBuffer :
	public Buffer
{
public:
	InputStructuredBuffer(Graphics& gfx, const vector<T>& data, ShaderDataTypes type, UINT slot);
	void Bind(Graphics& gfx) const override;
	void WriteToBuffer(Graphics& gfx, const vector<T>& data);
private:
	ComPtr<ID3D11ShaderResourceView> pSRV;
	const ShaderDataTypes shaderType;
	const UINT numElements;
};

