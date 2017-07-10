#pragma once

#include "../framework/framework.h"
#include "irenderermaster.h"

namespace flounder {
	class renderer :
		public imodule
	{
	private:
		irenderermaster *m_renderermaster;
	public:
		static renderer *get()
		{
			return (renderer*) framework::get()->getInstance("renderer");
		}

		renderer();
		~renderer();

		void loadRendererMaster(irenderermaster *renderermaster);

		void init();

		void update();

		inline irenderermaster *getRendererMaster() { return m_renderermaster; }
	};
}
