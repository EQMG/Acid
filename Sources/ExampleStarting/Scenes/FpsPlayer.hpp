#pragma once

#include <Objects/Behaviour.hpp>
#include <Objects/GameObject.hpp>
#include <Inputs/IAxis.hpp>
#include <Inputs/IButton.hpp>

using namespace Flounder;

namespace Demo
{
	class FpsPlayer :
		public Behaviour
	{
	private:
		static const float WALK_SPEED;
		static const float RUN_SPEED;
		static const float CROUCH_SPEED;
		static const float JUMP_SPEED;
		static const float NOCLIP_SPEED;

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
	};
}
