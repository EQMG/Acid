#pragma once

#include "renderer/irenderermaster.h"
#include "renderer/renderer.h"

#include "devices/mouse.h"
#include "devices/keyboard.h"
#include "deferred/deferredrenderer.h"
#include "fbos/fbo.h"
#include "post/filters/filterfxaa.h"
#include "post/filters/filtergrain.h"
#include "post/filters/filtercrt.h"
#include "skybox/rendererskybox.h"

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
