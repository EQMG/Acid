#pragma once

#include <Objects/GameObject.hpp>
#include <Objects/IComponent.hpp>
#include <Inputs/AxisCompound.hpp>
#include <Inputs/ButtonCompound.hpp>

using namespace acid;

namespace test
{
	class FpsPlayer :
		public IComponent
	{
	private:
		bool m_noclipEnabled;

		AxisCompound m_inputForward;
		AxisCompound m_inputStrafe;
		ButtonCompound m_inputSprint;
		ButtonCompound m_inputJump;
		ButtonCompound m_inputCrouch;
		ButtonCompound m_toggleNoclip;
	public:
		FpsPlayer();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	};
}
