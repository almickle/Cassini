#include "PointLight.h"

PhongLightingData PointLight::GetLightData() {
	return {
		GetPosition(),
		color,
		ambient,
	};
}

void PointLight::UpdateLight(Graphics& gfx) {
	gfx.SetLighting(GetLightData());
}