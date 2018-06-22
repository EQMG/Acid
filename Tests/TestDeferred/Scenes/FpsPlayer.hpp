#pragma once

#include <Objects/IBehaviour.hpp>
#include <Objects/GameObject.hpp>
#include <Inputs/IAxis.hpp>
#include <Inputs/IButton.hpp>

using namespace fl;

namespace test
{
	class FpsPlayer :
		public IBehaviour
	{
	private:
		Vector3 m_velocity;
		bool m_jumping;
		bool m_noclipEnabled;

		IAxis *m_inputForward;
		IAxis *m_inputStrafe;
		IButton *m_inputSprint;
		IButton *m_inputJump;
		IButton *m_inputCrouch;
		IButton *m_toggleNoclip;

		Vector3 m_amountMove;
		Vector3 m_amountRotate;
	public:
		FpsPlayer();

		~FpsPlayer();

		void Update() override;
	};
}
