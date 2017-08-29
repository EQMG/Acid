#pragma once

#include "../ipostfilter.hpp"

namespace flounder
{
	class filterfxaa :
		public ipostfilter
	{
	private:
		float m_spanMax;
	public:
		filterfxaa(const float &spanMax = 8.0f);

		~filterfxaa();

		void storeValues() override;

		inline void setSpanMax(const float &spanMax) { m_spanMax = spanMax; }
	};
}
