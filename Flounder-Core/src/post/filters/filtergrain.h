#pragma once

#include "../ipostfilter.h"

namespace flounder 
{
	class filtergrain :
		public ipostfilter
	{
	private:
		float m_strength;
	public:
		filtergrain(const float &strength);

		filtergrain();

		~filtergrain();

		void storeValues() override;

		inline float getStrength() { return m_strength; }

		inline void setStrength(const float &strength) { m_strength = strength; }
	};
}
