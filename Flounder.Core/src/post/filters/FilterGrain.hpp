#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtergrain :
		public IPostFilter
	{
	private:
		float m_strength;
	public:
		filtergrain(const float &strength = 2.3f);

		~filtergrain();

		void StoreValues() override;

		void setStrength(const float &strength) { m_strength = strength; }
	};
}
