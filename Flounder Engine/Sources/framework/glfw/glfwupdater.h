#pragma once

#include "../updater.h"

#include "../../camera/camera.h"
#include "../../devices/audio.h"
#include "../../devices/display.h"
#include "../../devices/joysticks.h"
#include "../../devices/keyboard.h"
#include "../../devices/mouse.h"
#include "../../logger/logger.h"

namespace flounder {
	class glfwupdater : public updater
	{
	private:
		logger *m_logger;
		display *m_display;
		joysticks *m_joysticks;
		keyboard *m_keyboard;
		mouse *m_mouse;
		audio *m_audio;
		camera *m_camera;

	public:
		glfwupdater();

		~glfwupdater();

		void init() override;

		void update() override;

		module *getInstance(std::string name) override;
	};
}
