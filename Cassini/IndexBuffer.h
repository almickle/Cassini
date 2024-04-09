#pragma once
#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(Graphics& gfx, const vector<unsigned short>& indices);
	void Bind(Graphics& gfx) const override;
	UINT GetCount() const;
private:
	UINT count;
};