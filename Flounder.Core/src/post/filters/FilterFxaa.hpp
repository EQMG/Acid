#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterfxaa :
		public IPostFilter
	{
	private:
		float m_spanMax;
	public:
		filterfxaa(const float &spanMax = 8.0f);

		~filterfxaa();

		void StoreValues() override;

		void setSpanMax(const float &spanMax) { m_spanMax = spanMax; }
	};
}
