#pragma once
#include "Buffer.h"

template<typename T>
class StaticConstantBuffer :
	public Buffer
{
public:
	StaticConstantBuffer(Graphics& gfx, const T& cbData, UINT slot);
	void Bind(Graphics& gfx) const override;
};

