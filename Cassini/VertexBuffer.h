#pragma once
#include "Buffer.h"

template<typename V>
class VertexBuffer : public Buffer
{
public:
	VertexBuffer(Graphics& gfx, const vector<V>& vertices, UINT slot);
	void Bind(Graphics& gfx) const override;
private:
	UINT stride;
	UINT offset = 0u;
};