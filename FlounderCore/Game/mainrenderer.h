#pragma once

#include "../Sources/renderer/irenderermaster.h"
#include "../Sources/renderer/renderer.h"

#include "../Sources/devices/keyboard.h"
#include "../Sources/deferred/deferredrenderer.h"
#include "../Sources/fbos/fbo.h"
#include "../Sources/post/filters/filterfxaa.h"
#include "../Sources/post/filters/filtergrain.h"
#include "../Sources/post/filters/filtercrt.h"
#include "../Sources/skybox/rendererskybox.h"

namespace flounder {
	class mainrenderer : public irenderermaster
	{
	private:
		vector4 *m_infinity;

		rendererskybox *m_skybox;

		fbo *m_fboRenderer;
		deferredrenderer *m_deferred;
		filterfxaa *m_filterFxaa;
		filtergrain *m_filterGrain;
		filtercrt *m_filterCrt;
	public:
		mainrenderer();

		~mainrenderer();

		void render() override;
	};
}
