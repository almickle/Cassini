#pragma once
#include "Buffer.h"

template<typename T>
class OutputStructuredBuffer :
	public Buffer
{
public:
	OutputStructuredBuffer(Graphics& gfx, const vector<T>& data, UINT slot);
	void Bind(Graphics& gfx) const override;
	vector<T> ReadFromBuffer(Graphics& gfx) const;
private:
	ComPtr<ID3D11UnorderedAccessView> pUAV;
	const UINT numElements;
};

