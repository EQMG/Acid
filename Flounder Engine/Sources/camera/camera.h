#pragma once

#include <iostream>
#include "../framework/framework.h"

#include "icamera.h"
#include "iplayer.h"

namespace flounder {
	class camera : public imodule
	{
	private:
		icamera *m_camera;
		iplayer *m_player;
	public:
		static camera *get()
		{
			return (camera*) framework::get()->getInstance("camera");
		}

		camera();

		~camera();

		void loadCamera(icamera *camera);

		void loadPlayer(iplayer *player);

		void update();

		inline icamera *getCamera() { return m_camera; }

		inline iplayer *getPlayer() { return m_player; }
	};
}
