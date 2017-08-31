#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtergrain :
		public ipostfilter
	{
	private:
		float m_strength;
	public:
		filtergrain(const float &strength = 2.3f);

		~filtergrain();

		void storeValues() override;

		void setStrength(const float &strength) { m_strength = strength; }
	};
}
