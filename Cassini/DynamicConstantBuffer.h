#pragma once
#include "Buffer.h"
#include "InstanceData.h"

template<typename T>
class DynamicConstantBuffer : public Buffer {
public:
	DynamicConstantBuffer(Graphics& gfx, const T& cbData, ShaderDataTypes type, UINT slot);
	void Bind(Graphics& gfx) const override;
	void Update(Graphics& gfx, const T& cbData);
private:
	const ShaderDataTypes shaderType;
};