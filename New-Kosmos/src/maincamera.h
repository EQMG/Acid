#pragma once

#include <flounder.h>

using namespace flounder;

class maincamera :
	public icamera
{
private:
	static const float NEAR_PLANE;
	static const float FAR_PLANE;
	static const float FIELD_OF_VIEW;

	static const float ZOOM_AGILITY;
	static const float ROTATE_AGILITY;
	static const float PITCH_AGILITY;

	static const float INFLUENCE_OF_JOYSTICK_DY;
	static const float INFLUENCE_OF_JOYSTICK_DX;
	static const float INFLUENCE_OF_JOYSTICK_ZOOM;

	static const float INFLUENCE_OF_MOUSE_DY;
	static const float INFLUENCE_OF_MOUSE_DX;
	static const float INFLUENCE_OF_MOUSE_WHEEL;

	static const float MAX_HORIZONTAL_CHANGE;
	static const float MAX_VERTICAL_CHANGE;
	static const float MAX_ZOOM_CHANGE;

	static const float MAX_ANGLE_OF_ELEVATION;
	static const float MIN_ANGLE_OF_ELEVATION;
	static const float MINIMUM_ZOOM;
	static const float MAXIMUM_ZOOM;
	static const float NORMAL_ZOOM;

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
	float m_targetZoom;
	float m_targetElevation;
	float m_targetRotationAngle;

	float m_actualDistanceFromPoint;
	float m_horizontalDistanceFromFocus;
	float m_verticalDistanceFromFocus;

	float m_sensitivity;
	int m_reangleButton;
	axisjoystick *m_joystickVertical;
	axisjoystick *m_joystickHorizontal;
	buttonjoystick *m_joystickZoom;
public:
	maincamera();

	~maincamera();

	void update(const iplayer &player) override;
private:
	void calculateHorizontalAngle();

	void calculateVerticalAngle();

	void calculateZoom();

	void updateHorizontalAngle(const float &delta);

	void updatePitchAngle(const float &delta);

	void updateActualZoom(const float &delta);

	void updateDistances();

	void updatePosition();
public:
	void reflect(const float &waterHeight) override;

	float getNearPlane() const override { return NEAR_PLANE; }

	float getFarPlane() const override { return FAR_PLANE; }

	float getFOV() const override { return FIELD_OF_VIEW; }

	inline frustum *getViewFrustum() const override { return m_viewFrustum; }

	inline ray *getViewRay() const override { return m_viewRay; }

	inline matrix4x4 *getViewMatrix() const override { return m_viewMatrix; }

	inline matrix4x4 *getProjectionMatrix() const override { return m_projectionMatrix; }

	inline vector3 *getPosition() const override { return m_position; }

	inline vector3 *getRotation() const override { return m_rotation; }
};
