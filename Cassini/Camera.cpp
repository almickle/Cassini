#include "Camera.h"

void Camera::UpdateViewMatrix(Graphics& gfx, XMMATRIX transform) {
	gfx.SetCameraTransform(transform);
};