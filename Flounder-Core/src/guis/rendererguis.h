#pragma once

#include "../loaders/loaders.h"
#include "../renderer/irenderer.h"

#include "guis.h"
#include "guiobject.h"

namespace flounder
{
	class rendererguis :
		public irenderer
	{
	private:
		shader *m_shader;

		GLuint m_vaoID;
		GLuint m_vaoLength;
	public:
		rendererguis();

		~rendererguis();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void renderGui(guiobject *object);

		void endRendering();
	};
}
