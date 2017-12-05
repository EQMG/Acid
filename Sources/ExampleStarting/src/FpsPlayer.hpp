#pragma once

#include <Flounder.hpp>

using namespace Flounder;

namespace Demo
{
	class FpsPlayer :
		public IPlayer
	{
	private:
		static const float WALK_SPEED;
		static const float RUN_SPEED;
		static const float CROUCH_SPEED;
		static const float JUMP_SPEED;
		static const float NOCLIP_SPEED;

		Vector3 *m_position;
		Vector3 *m_rotation;

		float m_currentSpeed;
		float m_currentStrafeSpeed;
		float m_currentUpwardSpeed;
		bool m_jumping;
		bool m_noclipEnabled;

		IAxis *m_inputForward;
		IAxis *m_inputStrafe;
		IButton *m_inputSprint;
		IButton *m_inputJump;
		IButton *m_inputCrouch;
		IButton *m_toggleNoclip;

		Vector3 *m_amountMove;
		Vector3 *m_amountRotate;

		bool m_paused;
	public:
		FpsPlayer();

		~FpsPlayer();

		void Update() override;

		inline Vector3 *GetPosition() const override { return m_position; }

		inline Vector3 *GetRotation() const override { return m_rotation; }
	};
}
