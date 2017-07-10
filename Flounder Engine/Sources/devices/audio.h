#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "../framework/framework.h"
#include "../logger/logger.h"

#include "display.h"

namespace flounder {
	class audio : public imodule
	{
	private:
		ALCdevice *m_device;
		ALCcontext *m_context;
		int *buffers;
	public:
		static audio* get() 
		{
			return (audio*) framework::get()->getInstance("audio"); 
		}

		/// <summary>
		/// Creates a new GLFW sound manager.
		/// </summary>
		audio();

		/// <summary>
		/// Deconstructor for the sound manager.
		/// </summary>
		~audio();

		void init();

		void update();
	};
}
