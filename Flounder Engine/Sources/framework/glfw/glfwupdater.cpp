#include "glfwupdater.h"

namespace flounder {
	glfwupdater::glfwupdater()
	{
	}

	glfwupdater::~glfwupdater()
	{
		delete m_logger;
		delete m_display;
		delete m_joysticks;
		delete m_keyboard;
		delete m_mouse;
		delete m_audio;
		delete m_camera;
	}

	void glfwupdater::init()
	{
		m_logger = new logger();
		m_display = new display();
		m_joysticks = new joysticks();
		m_keyboard = new keyboard();
		m_mouse = new mouse;
		m_audio = new audio;
		m_camera = new camera();
	}

	void flounder::glfwupdater::update()
	{
		// Pre-Update
		m_joysticks->update();
		m_keyboard->update();
		m_mouse->update();
		m_audio->update();
		m_camera->update();

		// Update

		// Post-Update

		// Render
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(m_keyboard->getKey(GLFW_KEY_W), 0.0f, mouse::get()->getPositionY(), 1.0f);
		m_display->update();
	}

	module *flounder::glfwupdater::getInstance(std::string name)
	{
		if (name == "camera") {
			return m_camera;
		}
		else if (name == "audio") {
			return m_audio;
		}
		else if (name == "display") 
		{
			return m_display;
		}
		else if (name == "joysticks")
		{
			return m_joysticks;
		}
		else if (name == "keyboard")
		{
			return m_keyboard;
		}
		else if (name == "mouse")
		{
			return m_mouse;
		}
		else if (name == "logger")
		{
			return m_logger;
		}

		return NULL;
	}
}
