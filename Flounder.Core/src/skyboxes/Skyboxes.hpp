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
		Fog *m_fog;
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

		skybox *getSkybox() { return m_skybox; }

		void setSkybox(skybox *skybox) { m_skybox = skybox; }

		Fog *getFog() { return m_fog; }

		void setFog(Fog *fog) { m_fog = fog; }
	};
}
