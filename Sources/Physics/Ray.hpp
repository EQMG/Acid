#pragma once

#include "Maths/Matrix4.hpp"

namespace acid {
/**
 * @brief Class that represents a 3 dimensional ray.
 */
class ACID_EXPORT Ray {
public:
	/**
	 * Creates a new ray.
	 * @param useMouse If the ray will use the mouse coords or to start from screenStart.
	 * @param screenStart If useMouse is false then this will be used as the rays start.
	 */
	Ray(bool useMouse, const Vector2f &screenStart);

	/**
	 * Updates the ray to a new position.
	 * @param currentPosition The origin of the ray.
	 * @param mousePosition The mouses screen space position.
	 * @param viewMatrix The cameras view matrix.
	 * @param projectionMatrix The projection view matrix.
	 */
	void Update(const Vector3f &currentPosition, const Vector2f &mousePosition, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

	/**
	 * Gets a point on the ray.
	 * @param distance Distance down the ray to sample.
	 * @return The destination vector.
	 */
	Vector3f GetPointOnRay(float distance) const;

	/**
	 * Converts a position from world space to screen space.
	 * @param position The position to convert.
	 * @return The destination vector X and Y being screen space coords and Z being the distance to the camera.
	 */
	Vector3f ConvertToScreenSpace(const Vector3f &position) const;

	bool IsUseMouse() const { return useMouse; }
	void SetUseMouse(bool useMouse) { this->useMouse = useMouse; }

	const Vector2f &GetScreenStart() const { return screenStart; }
	void SetScreenStart(const Vector2f &screenStart) { this->screenStart = screenStart; }

	const Vector3f &GetOrigin() const { return origin; }
	const Vector3f &GetCurrentRay() const { return currentRay; }

private:
	void UpdateNormalizedDeviceCoordinates(float mouseX, float mouseY);
	void UpdateEyeCoords();
	void UpdateWorldCoords();

	bool useMouse;
	Vector2f screenStart;

	Matrix4 projectionMatrix, viewMatrix;

	Vector2f normalizedCoords;
	Vector4f clipCoords;
	Vector4f eyeCoords;

	Matrix4 invertedProjection, invertedView;
	Vector4f rayWorld;

	Vector3f origin;
	Vector3f currentRay;
};
}
