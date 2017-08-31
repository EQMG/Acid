#pragma once

#include <flounder.hpp>

using namespace Flounder;

class maincamera :
	public ICamera
{
private:
	static const float NEAR_PLANE;
	static const float FAR_PLANE;
	static const float FIELD_OF_VIEW;

	static const float ROTATE_AGILITY;
	static const float PITCH_AGILITY;

	static const float INFLUENCE_OF_JOYSTICK_DY;
	static const float INFLUENCE_OF_JOYSTICK_DX;

	static const float INFLUENCE_OF_MOUSE_DY;
	static const float INFLUENCE_OF_MOUSE_DX;

	static const float MAX_HORIZONTAL_CHANGE;
	static const float MAX_VERTICAL_CHANGE;

	static const float MAX_ANGLE_OF_ELEVATION;
	static const float MIN_ANGLE_OF_ELEVATION;

	vector3 *m_position;
	vector3 *m_rotation;

	matrix4x4 *m_viewMatrix;
	matrix4x4 *m_projectionMatrix;

	frustum *m_viewFrustum;
	ray *m_viewRay;

	float m_angleOfElevation;
	float m_angleAroundPlayer;

	vector3 *m_targetPosition;
	vector3 *m_targetRotation;
	float m_targetElevation;
	float m_targetRotationAngle;

	float m_sensitivity;
	int m_reangleButton;
	axisjoystick *m_joystickVertical;
	axisjoystick *m_joystickHorizontal;
	buttonjoystick *m_joystickZoom;

	bool m_paused;
public:
	maincamera();

	~maincamera();

	void Update(IPlayer *player) override;
private:
	void calculateHorizontalAngle();

	void calculateVerticalAngle();

	void updateHorizontalAngle(const float &delta);

	void updatePitchAngle(const float &delta);

	void updatePosition();
public:
	void ReflectView(const float &waterHeight) override;

	float GetNearPlane() const override { return NEAR_PLANE; }

	float GetFarPlane() const override { return FAR_PLANE; }

	float GetFov() const override { return FIELD_OF_VIEW; }

	inline frustum *GetViewFrustum() const override { return m_viewFrustum; }

	inline ray *GetViewRay() const override { return m_viewRay; }

	inline matrix4x4 *GetViewMatrix() const override { return m_viewMatrix; }

	inline matrix4x4 *GetProjectionMatrix() const override { return m_projectionMatrix; }

	inline vector3 *GetPosition() const override { return m_position; }

	inline vector3 *GetRotation() const override { return m_rotation; }
};
