#pragma once

#include <Objects/IBehaviour.hpp>
#include <Objects/GameObject.hpp>
#include <Inputs/IAxis.hpp>
#include <Inputs/IButton.hpp>

using namespace acid;

namespace test
{
	class FpsPlayer :
		public IBehaviour
	{
	private:
		Vector3 m_velocity;
		bool m_jumping;
		bool m_noclipEnabled;

		std::shared_ptr<IAxis> m_inputForward;
		std::shared_ptr<IAxis> m_inputStrafe;
		std::shared_ptr<IButton> m_inputSprint;
		std::shared_ptr<IButton> m_inputJump;
		std::shared_ptr<IButton> m_inputCrouch;
		std::shared_ptr<IButton> m_toggleNoclip;

		Vector3 m_amountMove;
		Vector3 m_amountRotate;
	public:
		FpsPlayer();

		~FpsPlayer();

		void Update() override;
	};
}
