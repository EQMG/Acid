#pragma once

#include "../../camera/camera.hpp"
#include "../../devices/display.hpp"
#include "../../maths/matrix4x4.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtermotion :
		public ipostfilter
	{
	private:
		matrix4x4 *m_lastViewMatrix;
	public:
		filtermotion();

		~filtermotion();

		void storeValues() override;
	};
}
