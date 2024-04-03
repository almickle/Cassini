#include "PointLight.h"

PhongLightingData PointLight::GetLightData() {
	return {
		GetPosition(),
		color,
		ambient,
		intensity,
		attConst
	};
}

void PointLight::UpdateLight(Graphics& gfx) {
	gfx.SetLighting(GetLightData());
}