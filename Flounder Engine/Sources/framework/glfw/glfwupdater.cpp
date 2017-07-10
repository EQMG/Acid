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
		delete m_events;
		delete m_tasks;
		delete m_processing;
		delete m_display;
		delete m_joysticks;
		delete m_keyboard;
		delete m_mouse;
		delete m_audio;
		delete m_standards;
		delete m_camera;
		delete m_shaders;
		delete m_textures;
		delete m_models;
		delete m_loaders;
		delete m_renderer;
	}

	void glfwupdater::create()
	{
		m_startTime = glfwGetTime();
		m_timeOffset = 0.0;
		m_deltaUpdate = new delta();
		m_deltaRender = new delta();
		m_timerUpdate = new timer(1.0 / 60.0);
		m_timerRender = new timer(1.0 / framework::get()->getFpsLimit());
		m_timerLog = new timer(1.0);

		m_logger = new logger();
		m_events = new events();
		m_tasks = new tasks();
		m_processing = new processing();
		m_display = new display();
		m_joysticks = new joysticks();
		m_keyboard = new keyboard();
		m_mouse = new mouse;
		m_audio = new audio;
		m_standards = new standards();
		m_camera = new camera();
		m_shaders = new shaders();
		m_textures = new textures();
		m_models = new models();
		m_loaders = new loaders();
		m_renderer = new renderer();
	}

	void glfwupdater::init()
	{
		m_logger->init();

		m_display->init();
		m_joysticks->init();
		m_keyboard->init();
		m_mouse->init();
		m_audio->init();

		m_standards->init();

		m_events->init();
		m_tasks->init();
		m_processing->init();

		m_camera->init();
		m_shaders->init();
		m_textures->init();
		m_models->init();
		m_loaders->init();
		m_renderer->init();
	}

	void flounder::glfwupdater::update()
	{
		// Always-Update
		m_logger->update();

		if (m_timerUpdate->isPassedTime())
		{
			// Resets the timer.
			m_timerUpdate->resetStartTime();

			// Updates the frameworks delta.
			m_deltaUpdate->update();

			// Pre-Update
			m_joysticks->update();
			m_keyboard->update();
			m_mouse->update();
			m_audio->update();
			m_camera->update();

			m_standards->update();

			m_events->update();
			m_tasks->update();
			m_processing->update();

			m_shaders->update();
			m_textures->update();
			m_models->update();
			m_loaders->update();

			// Update

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
			m_renderer->update();
			m_display->update();
		}
	}

	imodule *flounder::glfwupdater::getInstance(std::string name)
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
		else if (name == "events")
		{
			return m_events;
		}
		else if (name == "loaders")
		{
			return m_loaders;
		}
		else if (name == "logger")
		{
			return m_logger;
		}
		else if (name == "models")
		{
			return m_models;
		}
		else if (name == "renderer")
		{
			return m_renderer;
		}
		else if (name == "shaders")
		{
			return m_shaders;
		}
		else if (name == "standards")
		{
			return m_standards;
		}
		else if (name == "processing")
		{
			return m_processing;
		}
		else if (name == "tasks")
		{
			return m_tasks;
		}
		else if (name == "textures")
		{
			return m_textures;
		}

		return NULL;
	}
}
