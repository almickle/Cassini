#include "Camera.h"

const string Camera::entityID = "Camera";

void Camera::UpdateViewMatrix(Graphics& gfx, XMMATRIX transform) {
	gfx.SetCameraTransform(transform);
};