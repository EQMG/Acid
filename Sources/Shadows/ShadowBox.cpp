#include "ShadowBox.hpp"

#include "Devices/Window.hpp"
#include "Graphics/Graphics.hpp"
#include "Maths/Maths.hpp"

namespace acid {
ShadowBox::ShadowBox() {
	// Creates the offset for part of the conversion to shadow map space.
	offset = offset.Translate(Vector3f(0.5f));
	offset = offset.Scale(Vector3f(0.5f));
}

void ShadowBox::Update(const Camera &camera, const Vector3f &lightDirection, float shadowOffset, float shadowDistance) {
	this->lightDirection = lightDirection;
	this->shadowOffset = shadowOffset;
	this->shadowDistance = shadowDistance;

	UpdateShadowBox(camera);
	UpdateOrthoProjectionMatrix();
	UpdateCenter();
	UpdateLightViewMatrix();
	UpdateViewShadowMatrix();
}

bool ShadowBox::IsInBox(const Vector3f &position, float radius) const {
	auto entityPos = lightViewMatrix.Transform(Vector4f(position));

	Vector3f closestPoint;
	closestPoint.x = std::clamp(entityPos.x, minExtents.x, maxExtents.x);
	closestPoint.y = std::clamp(entityPos.y, minExtents.y, maxExtents.y);
	closestPoint.z = std::clamp(entityPos.z, minExtents.z, maxExtents.z);

	Vector3f centre(entityPos);
	auto distance = centre - closestPoint;
	auto distanceSquared = distance.LengthSquared();

	return distanceSquared < radius * radius;
}

void ShadowBox::UpdateShadowBox(const Camera &camera) {
	UpdateSizes(camera);

	Matrix4 rotation;
	rotation = rotation.Rotate(-camera.GetRotation().y, Vector3f::Up);
	rotation = rotation.Rotate(-camera.GetRotation().x, Vector3f::Right);

	Vector3f forwardVector(rotation.Transform(Vector4f(0.0f, 0.0f, -1.0f, 0.0f)));

	auto toFar = forwardVector * shadowDistance;
	auto toNear = forwardVector * camera.GetNearPlane();
	auto centreNear = toNear + camera.GetPosition();
	auto centreFar = toFar + camera.GetPosition();

	auto points = CalculateFrustumVertices(rotation, forwardVector, centreNear, centreFar);

	minExtents = Vector3f::PositiveInfinity;
	maxExtents = Vector3f::NegativeInfinity;

	for (const auto &point : points) {
		Vector3f extent(point);
		minExtents = minExtents.Min(extent);
		maxExtents = minExtents.Max(extent);
	}

	maxExtents.z += shadowOffset;
}

void ShadowBox::UpdateSizes(const Camera &camera) {
	farWidth = shadowDistance * std::tan(camera.GetFieldOfView());
	nearWidth = camera.GetNearPlane() * std::tan(camera.GetFieldOfView());
	farHeight = farWidth / Window::Get()->GetAspectRatio();
	nearHeight = nearWidth / Window::Get()->GetAspectRatio();
}

std::array<Vector4f, 8> ShadowBox::CalculateFrustumVertices(const Matrix4 &rotation, const Vector3f &forwardVector, const Vector3f &centreNear, const Vector3f &centreFar) const {
	Vector3f upVector(rotation.Transform(Vector4f(0.0f, 1.0f, 0.0f, 0.0f)));
	auto rightVector = forwardVector.Cross(upVector);
	auto downVector = -upVector;
	auto leftVector = -rightVector;

	auto farTop = centreFar + (upVector * farHeight);
	auto farBottom = centreFar + (downVector * farHeight);
	auto nearTop = centreNear + (upVector * nearHeight);
	auto nearBottom = centreNear + (downVector * nearHeight);

	std::array<Vector4f, 8> points;
	points[0] = CalculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
	points[1] = CalculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
	points[2] = CalculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
	points[3] = CalculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
	points[4] = CalculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
	points[5] = CalculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
	points[6] = CalculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
	points[7] = CalculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
	return points;
}

Vector4f ShadowBox::CalculateLightSpaceFrustumCorner(const Vector3f &startPoint, const Vector3f &direction, float width) const {
	Vector4f point(startPoint + (direction * width));
	point = lightViewMatrix.Transform(point);
	return point;
}

void ShadowBox::UpdateOrthoProjectionMatrix() {
	projectionMatrix = {};
	projectionMatrix[0][0] = 2.0f / GetWidth();
	projectionMatrix[1][1] = 2.0f / GetHeight();
	projectionMatrix[2][2] = -2.0f / GetDepth();
	projectionMatrix[3][3] = 1.0f;
}

void ShadowBox::UpdateCenter() {
	Vector4f centre((minExtents + maxExtents) / 2.0f);
	auto invertedLight = lightViewMatrix.Inverse();
	centre = {invertedLight.Transform(centre)};
}

void ShadowBox::UpdateLightViewMatrix() {
	lightViewMatrix = {};
	auto pitch = std::acos(Vector2f(lightDirection.x, lightDirection.z).Length());
	lightViewMatrix = lightViewMatrix.Rotate(pitch, Vector3f::Right);
	auto yaw = std::atan(lightDirection.x / lightDirection.z);

	if (lightDirection.z > 0.0f) {
		yaw -= Maths::PI<float>;
	}

	lightViewMatrix = lightViewMatrix.Rotate(-yaw, Vector3f::Up);
	lightViewMatrix = lightViewMatrix.Translate(centre);
}

void ShadowBox::UpdateViewShadowMatrix() {
	projectionViewMatrix = projectionMatrix * lightViewMatrix;
	shadowMapSpaceMatrix = offset * projectionViewMatrix;
}
}
