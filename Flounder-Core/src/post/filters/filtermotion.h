#pragma once

#include "../../camera/camera.h"
#include "../../devices/display.h"
#include "../../maths/matrix4x4.h"
#include "../ipostfilter.h"

namespace flounder
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
