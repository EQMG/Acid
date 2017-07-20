#pragma once

#include "../ipostfilter.h"

namespace flounder 
{
	class filterfxaa :
		public ipostfilter
	{
	private:
		float m_spanMax;
	public:
		filterfxaa(const float &spanMax);

		filterfxaa();

		~filterfxaa();

		void storeValues() override;

		inline float getSpanMax() { return m_spanMax; }

		inline void setSpanMax(const float &spanMax) { m_spanMax = spanMax; }
	};
}
