#pragma once

#include "../framework/framework.h"
#include "../lights/fog.h"

#include "skybox.h"

namespace flounder
{
	/// <summary>
	/// A module used for managing skyboxes in 3D worlds.
	/// </summary>
	class skyboxes :
		public imodule
	{
	private:
		skybox *m_skybox;
		fog *m_fog;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static skyboxes *get()
		{
			return static_cast<skyboxes*>(framework::get()->getInstance("skyboxes"));
		}

		skyboxes();

		~skyboxes();

		void update() override;

		inline skybox *getSkybox() { return m_skybox; }

		inline void setSkybox(skybox *skybox) { m_skybox = skybox; }

		inline fog *getFog() { return m_fog; }

		inline void setFog(fog *fog) { m_fog = fog; }
	};
}
