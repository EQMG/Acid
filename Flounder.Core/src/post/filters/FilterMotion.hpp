#pragma once

#include "../../camera/camera.hpp"
#include "../../devices/Display.hpp"
#include "../../maths/Matrix4.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtermotion :
		public IPostFilter
	{
	private:
		Matrix4 *m_lastViewMatrix;
	public:
		filtermotion();

		~filtermotion();

		void StoreValues() override;
	};
}
