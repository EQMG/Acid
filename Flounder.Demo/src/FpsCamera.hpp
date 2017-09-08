#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class FpsCamera :
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

	Vector3 *m_position;
	Vector3 *m_rotation;

	Matrix4 *m_viewMatrix;
	Matrix4 *m_projectionMatrix;

	Frustum *m_viewFrustum;
	Ray *m_viewRay;

	float m_angleOfElevation;
	float m_angleAroundPlayer;

	Vector3 *m_targetPosition;
	Vector3 *m_targetRotation;
	float m_targetElevation;
	float m_targetRotationAngle;

	float m_sensitivity;
	int m_reangleButton;
	AxisJoystick *m_joystickVertical;
	AxisJoystick *m_joystickHorizontal;
	ButtonJoystick *m_joystickZoom;

	bool m_paused;
public:
	FpsCamera();

	~FpsCamera();

	void Update(IPlayer *player) override;
private:
	void CalculateHorizontalAngle();

	void CalculateVerticalAngle();

	void UpdateHorizontalAngle(const float &delta);

	void UpdatePitchAngle(const float &delta);

	void UpdatePosition();
public:
	void ReflectView(const float &waterHeight) override;

	float GetNearPlane() const override { return NEAR_PLANE; }

	float GetFarPlane() const override { return FAR_PLANE; }

	float GetFov() const override { return FIELD_OF_VIEW; }

	inline Frustum *GetViewFrustum() const override { return m_viewFrustum; }

	inline Ray *GetViewRay() const override { return m_viewRay; }

	inline Matrix4 *GetViewMatrix() const override { return m_viewMatrix; }

	inline Matrix4 *GetProjectionMatrix() const override { return m_projectionMatrix; }

	inline Vector3 *GetPosition() const override { return m_position; }

	inline Vector3 *GetRotation() const override { return m_rotation; }
};
