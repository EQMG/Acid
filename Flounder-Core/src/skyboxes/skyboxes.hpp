#pragma once

#include "../engine/Engine.hpp"
#include "../lights/fog.hpp"

#include "skybox.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing skyboxes in 3D worlds.
	/// </summary>
	class skyboxes :
		public IModule
	{
	private:
		skybox *m_skybox;
		fog *m_fog;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static skyboxes *get()
		{
			return static_cast<skyboxes*>(Engine::Get()->GetModule("skyboxes"));
		}

		skyboxes();

		~skyboxes();

		void Update() override;

		inline skybox *getSkybox() { return m_skybox; }

		inline void setSkybox(skybox *skybox) { m_skybox = skybox; }

		inline fog *getFog() { return m_fog; }

		inline void setFog(fog *fog) { m_fog = fog; }
	};
}
