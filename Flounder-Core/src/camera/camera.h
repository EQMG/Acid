#pragma once

#include "../framework/framework.h"

#include "icamera.h"
#include "iplayer.h"

namespace flounder 
{
	class camera : 
		public imodule
	{
	private:
		icamera *m_camera;
		iplayer *m_player;
	public:
		static camera *get()
		{
			return (camera*)framework::get()->getInstance("camera");
		}

		camera();

		~camera();

		void load(icamera *camera, iplayer *player);

		void init();

		void update();

		inline icamera *getCamera() const { return m_camera; }

		inline void setCamera(icamera *camera) { m_camera = camera; }

		inline iplayer *getPlayer() const { return m_player; }

		inline void setPlayer(iplayer *player) { m_player = player; }
	};
}
