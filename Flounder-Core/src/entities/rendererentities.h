#pragma once

#include "../loaders/loaders.h"
#include "../renderer/irenderer.h"
#include "../uis/uis.h"

#include "entities.h"

namespace flounder
{
	class rendererentities :
		public irenderer
	{
	private:
		shader *m_shader;
	public:
		rendererentities();

		~rendererentities();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void renderEntity(entity *object);

		void endRendering();
	};
}
