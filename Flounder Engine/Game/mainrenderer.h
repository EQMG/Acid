#pragma once

#include "../Sources/renderer/irenderermaster.h"
#include "../Sources/renderer/renderer.h"

#include "../Sources/fbos/fbo.h"
#include "../Sources/visual/driverbounce.h"
#include "../Sources/visual/driversinwave.h"

#include "../Sources/skybox/rendererskybox.h"

namespace flounder {
	class mainrenderer : public irenderermaster
	{
	private:
		vector4 *m_infinity;

		rendererskybox *m_skybox;

		fbo *m_fboRenderer;

		idriver *m_driverR;
		idriver *m_driverB;
	public:
		mainrenderer();

		~mainrenderer();

		void render() override;
	};
}
