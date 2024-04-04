#include "PointLight.h"

PhongLightingData PointLight::ComputeLightData(Graphics& gfx) {
	XMFLOAT3 pos = GetPosition();
	XMVECTOR transformed = XMVector3Transform(XMLoadFloat3(&pos), gfx.GetCameraView());
	XMStoreFloat3(&pos, transformed);
	return {
		pos,
		color,
		ambient,
	};
}

void PointLight::UpdateLight(Graphics& gfx) {
	SetPosition({ radius * cos((theta + 180.0f) * 3.14f / 180.0f), height, 10.0f + radius * sin((theta + 180.0f) * 3.14f / 180.0f) });
	gfx.SetLighting(ComputeLightData(gfx));
}