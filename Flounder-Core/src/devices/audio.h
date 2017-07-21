#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "../camera/camera.h"
#include "../framework/framework.h"
#include "../maths/vector3.h"

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
		ALCdevice *m_device;
		ALCcontext *m_context;
		int *buffers;
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

		void init() override;

		void update() override;
	};
}
