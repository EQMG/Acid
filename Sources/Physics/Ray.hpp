#pragma once

#include "Maths/Matrix4.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"

namespace acid
{
/**
 * @brief Class that represents a 3 dimensional ray.
 */
class ACID_EXPORT Ray
{
public:
	static const Ray Zero;

	/**
	 * Creates a new ray.
	 * @param useMouse If the ray will use the mouse coords or to start from screenStart.
	 * @param screenStart If useMouse is false then this will be used as the rays start.
	 */
	Ray(const bool &useMouse, const Vector2f &screenStart);

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
	Vector3f GetPointOnRay(const float &distance) const;

	/**
	 * Converts a position from world space to screen space.
	 * @param position The position to convert.
	 * @return The destination vector X and Y being screen space coords and Z being the distance to the camera.
	 */
	Vector3f ConvertToScreenSpace(const Vector3f &position) const;

	const bool &IsUseMouse() const { return m_useMouse; }

	void SetUseMouse(const bool &useMouse) { m_useMouse = useMouse; }

	const Vector2f &GetScreenStart() const { return m_screenStart; }

	void SetScreenStart(const Vector2f &screenStart) { m_screenStart = screenStart; }

	const Vector3f &GetOrigin() const { return m_origin; }

	const Vector3f &GetCurrentRay() const { return m_currentRay; }

private:
	void UpdateNormalisedDeviceCoordinates(const float &mouseX, const float &mouseY);

	void UpdateEyeCoords();

	void UpdateWorldCoords();

	bool m_useMouse;
	Vector2f m_screenStart;

	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;

	Vector2f m_normalizedCoords;
	Vector4f m_clipCoords;
	Vector4f m_eyeCoords;

	Matrix4 m_invertedProjection;
	Matrix4 m_invertedView;
	Vector4f m_rayWorld;

	Vector3f m_origin;
	Vector3f m_currentRay;
};
}
