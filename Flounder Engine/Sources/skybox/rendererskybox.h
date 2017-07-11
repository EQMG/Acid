#pragma once

#include "../renderer/irenderer.h" 
#include "skybox.h"

namespace flounder {
	class rendererskybox : public irenderer
	{
	private:
		shader *m_shader;
	public:
		rendererskybox();

		~rendererskybox();

		void render(vector4 *clipPlane, icamera *camera);
	};
}
