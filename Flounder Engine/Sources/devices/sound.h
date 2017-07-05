#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "../framework/framework.h"
#include "../logger/logger.h"

#include "display.h"

namespace flounder {
	class sound : public module
	{
	private:
		ALCdevice *m_device;
		ALCcontext *m_context;
		int *buffers;
	public:
		static sound* get() {
			return static_cast<sound*>(framework::get()->getInstance("sound"));
		}

		/// <summary>
		/// Creates a new GLFW sound manager.
		/// </summary>
		sound();

		/// <summary>
		/// Deconstructor for the sound manager.
		/// </summary>
		~sound();

		void init();

		void update();
	};
}
