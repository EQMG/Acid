#pragma once

#include "devices/display.h"
#include "devices/mouse.h"
#include "camera/icamera.h"

using namespace flounder;

class maincamera :
	public icamera
{
private:
	vector3 *m_position;
	vector3 *m_rotation;

	matrix4x4 *m_viewMatrix;
	matrix4x4 *m_projectionMatrix;

	frustum *m_viewFrustum;
	ray *m_viewRay;
public:
	maincamera();

	~maincamera();

	void update(const iplayer &player) override;

	void reflect(const float &waterHeight) override;

	float getNearPlane() const override { return 0.1f; }

	float getFarPlane() const override { return 1000.0f; }

	float getFOV() const override { return 60.0f; }

	inline frustum *getViewFrustum() const override { return m_viewFrustum; }

	inline ray *getViewRay() const override { return m_viewRay; }

	inline matrix4x4 *getViewMatrix() const override { return m_viewMatrix; }

	inline matrix4x4 *getProjectionMatrix() const override { return m_projectionMatrix; }

	inline vector3 *getPosition() const override { return m_position; }

	inline vector3 *getRotation() const override { return m_rotation; }
};
