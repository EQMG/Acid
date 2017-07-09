#include "glfwupdater.h"

namespace flounder {
	glfwupdater::glfwupdater()
	{
	}

	glfwupdater::~glfwupdater()
	{
		delete m_deltaUpdate;
		delete m_deltaRender;
		delete m_timerUpdate;
		delete m_timerRender;
		delete m_timerLog;

		delete m_logger;
		delete m_tasks;
		delete m_processing;
		delete m_display;
		delete m_joysticks;
		delete m_keyboard;
		delete m_mouse;
		delete m_audio;
		delete m_camera;

		delete m_buttonFullscreen;
		delete m_interpolation;
	}

	void glfwupdater::init()
	{
		m_startTime = glfwGetTime();
		m_timeOffset = 0.0;
		m_deltaUpdate = new delta();
		m_deltaRender = new delta();
		m_timerUpdate = new timer(1.0 / 60.0);
		m_timerRender = new timer(1.0 / framework::get()->getFpsLimit());
		m_timerLog = new timer(1.0);

		m_logger = new logger();
		m_tasks = new tasks();
		m_processing = new processing();
		m_display = new display();
		m_joysticks = new joysticks();
		m_keyboard = new keyboard();
		m_mouse = new mouse;
		m_audio = new audio;
		m_camera = new camera();

		m_buttonFullscreen = new buttonkeyboard(1, GLFW_KEY_F11);
		m_interpolation = new smoothfloat(0.5f, 1.0f);
	}

	void flounder::glfwupdater::update()
	{
		// Always-Update

		if (m_timerUpdate->isPassedTime())
		{
			// Resets the timer.
			m_timerUpdate->resetStartTime();

			// Updates the frameworks delta.
			m_deltaUpdate->update();

			// Pre-Update
			m_tasks->update();
			m_processing->update();

			m_joysticks->update();
			m_keyboard->update();
			m_mouse->update();
			m_audio->update();
			m_camera->update();

			// Update
			if (m_buttonFullscreen->wasDown()) {
				display::get()->setFullscreen(!display::get()->isFullscreen());
			}

			// Post-Update
		}

		// Logs the fps to the console.
		if (m_timerLog->isPassedTime()) {
			// Resets the timer.
			m_timerLog->resetStartTime();

			std::cout << "FPS: " << 1.0 / getDeltaRender() << ", UPS: " << 1.0 / getDelta() << std::endl;
		}

		// Renders when needed.
		// ( || framework::get()->getFpsLimit() == -1 || framework::get()->getFpsLimit() > 1000.0f) && maths::almostEqual(m_timerUpdate->getInterval(), m_deltaUpdate->getChange(), 10.0)
		if (m_timerRender->isPassedTime())
		{
			// Resets the timer.
			m_timerRender->resetStartTime();

			// Updates the render delta, and render time extension.
			m_deltaRender->update();

			// Render
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor((GLclampf) m_keyboard->getKey(GLFW_KEY_W), (GLclampf) 0.0, (GLclampf) mouse::get()->getPositionY(), (GLclampf) 1.0);
			m_display->update();
		}
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
