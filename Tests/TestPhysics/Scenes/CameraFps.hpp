#pragma once

#include <memory>
#include <Scenes/Camera.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Inputs/ButtonMouse.hpp>

using namespace acid;

namespace test
{
	class CameraFps :
		public Camera
	{
	private:
		static const float NEAR_PLANE;
		static const float FAR_PLANE;
		static const float FIELD_OF_VIEW;

		static const float ROTATE_AGILITY;
		static const float PITCH_AGILITY;

		static const float VIEW_HEIGHT;

		static const float INFLUENCE_OF_JOYSTICK_DY;
		static const float INFLUENCE_OF_JOYSTICK_DX;

		static const float INFLUENCE_OF_MOUSE_DY;
		static const float INFLUENCE_OF_MOUSE_DX;

		static const float MAX_HORIZONTAL_CHANGE;
		static const float MAX_VERTICAL_CHANGE;

		static const float MAX_ANGLE_OF_ELEVATION;
		static const float MIN_ANGLE_OF_ELEVATION;

		float m_angleOfElevation;
		float m_angleAroundPlayer;

		Vector3 m_targetPosition;
		Vector3 m_targetRotation;
		float m_targetElevation;
		float m_targetRotationAngle;

		float m_sensitivity;
		MouseButton m_reangleButton;
		AxisJoystick m_joystickVertical;
		AxisJoystick m_joystickHorizontal;

		bool m_paused;
	public:
		CameraFps();

		void Start() override;

		void Update() override;
	private:
		void CalculateHorizontalAngle();

		void CalculateVerticalAngle();

		void UpdateHorizontalAngle(const float &delta);

		void UpdatePitchAngle(const float &delta);

		void UpdatePosition();
	};
}
