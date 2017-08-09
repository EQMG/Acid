#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "../camera/camera.h"
#include "../framework/framework.h"
#include "../maths/vector3.h"
#include "../sounds/sound.h"

#include "display.h"

namespace flounder
{
	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class audio :
		public imodule
	{
	private:
		friend class sound;

		static std::vector<sound*> m_sounds;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static audio *get()
		{
			return static_cast<audio*>(framework::get()->getInstance("audio"));
		}

		/// <summary>
		/// Creates a new audio module.
		/// </summary>
		audio();

		/// <summary>
		/// Deconstructor for the audio module.
		/// </summary>
		~audio();

		void update() override;

		static sound *add(sound *object);

		static sound *get(const std::string &name);
	};
}
