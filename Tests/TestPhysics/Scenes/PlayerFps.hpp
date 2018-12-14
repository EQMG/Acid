#pragma once

#include <Objects/Entity.hpp>
#include <Objects/Component.hpp>
#include <Inputs/AxisCompound.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Physics/CollisionObject.hpp>

using namespace acid;

namespace test
{
	class PlayerFps :
		public Component
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
		PlayerFps();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	};
}
