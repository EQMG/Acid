#include "Ray.hpp"

namespace acid {
Ray::Ray(bool useMouse, const Vector2f &screenStart) :
	useMouse(useMouse),
	screenStart(screenStart) {
}

void Ray::Update(const Vector3f &currentPosition, const Vector2f &mousePosition, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix) {
	origin = currentPosition;

	if (useMouse) {
		UpdateNormalizedDeviceCoordinates(mousePosition.x, mousePosition.y);
	} else {
		normalizedCoords = screenStart;
	}

	this->viewMatrix = viewMatrix;
	this->projectionMatrix = projectionMatrix;
	clipCoords = {normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f};
	UpdateEyeCoords();
	UpdateWorldCoords();
}

Vector3f Ray::GetPointOnRay(float distance) const {
	auto vector = distance * currentRay;
	return origin + vector;
}

Vector3f Ray::ConvertToScreenSpace(const Vector3f &position) const {
	Vector4f coords(position, 1.0f);
	coords = viewMatrix.Transform(coords);
	coords = projectionMatrix.Transform(coords);

	if (coords.w < 0.0f)
		return {};
	return {(coords.x / coords.w + 1.0f) / 2.0f, 1.0f - (coords.y / coords.w + 1.0f) / 2.0f, coords.z};
}

void Ray::UpdateNormalizedDeviceCoordinates(float mouseX, float mouseY) {
	normalizedCoords.x = (2.0f * mouseX) - 1.0f;
	normalizedCoords.y = (2.0f * mouseY) - 1.0f;
}

void Ray::UpdateEyeCoords() {
	invertedProjection = projectionMatrix.Inverse();
	eyeCoords = invertedProjection.Transform(clipCoords);
	eyeCoords = {eyeCoords.x, eyeCoords.y, -1.0f, 0.0f};
}

void Ray::UpdateWorldCoords() {
	invertedView = viewMatrix.Inverse();
	rayWorld = invertedView.Transform(eyeCoords);
	currentRay = rayWorld.xyz();
}
}
