#include "glfwupdater.h"

namespace flounder
{
	glfwupdater::glfwupdater() :
		iupdater()
	{
	}

	glfwupdater::~glfwupdater()
	{
		delete m_audio;
		delete m_camera;
		delete m_deltaRender;
		delete m_deltaUpdate;
		delete m_display;
		delete m_events;
		delete m_joysticks;
		delete m_keyboard;
		delete m_loaders;
		delete m_mouse;
		delete m_entities;
		delete m_processing;
		delete m_renderer;
		delete m_standards;
		delete m_tasks;
		delete m_uis;
		delete m_particles;

		delete m_timerRender;
		delete m_timerUpdate;
		delete m_skyboxes;
	}

	void glfwupdater::create()
	{
		m_startTime = glfwGetTime();
		m_timeOffset = 0.0;
		m_deltaUpdate = new delta();
		m_deltaRender = new delta();
		m_timerUpdate = new timer(1.0 / 62.0);
		m_timerRender = new timer(1.0 / -1.0);

		m_display = new display();
		m_audio = new audio;
		m_camera = new camera();
		m_events = new events();
		m_joysticks = new joysticks();
		m_keyboard = new keyboard();
		m_loaders = new loaders();
		m_mouse = new mouse;
		m_entities = new entities();
		m_processing = new processing();
		m_renderer = new renderer();
		m_standards = new standards();
		m_tasks = new tasks();
		m_uis = new uis();
		m_particles = new particles();
		m_skyboxes = new skyboxes;
	}

	void flounder::glfwupdater::update()
	{
		this->m_timerRender->setInterval(1.0 / display::get()->getFpsLimit());

		// Always-Update

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

			m_loaders->update();
			m_uis->update();

			// Update
			m_entities->update();
			m_particles->update();
			m_skyboxes->update();

			// Post-Update
		}

		// Renders when needed.
		if ((m_timerRender->isPassedTime() || display::get()->getFpsLimit() <= 0.0f || display::get()->getFpsLimit() > 1000.0f) && maths::almostEqual(m_timerUpdate->getInterval(), m_deltaUpdate->getChange(), 7.0))
		// if (display::get()->getFpsLimit() <= 0.0f || m_timerRender->isPassedTime())
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

	imodule *flounder::glfwupdater::getInstance(const std::string &name)
	{
		if (name == "camera")
		{
			return m_camera;
		}
		else if (name == "audio")
		{
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
		else if (name == "entities")
		{
			return m_entities;
		}
		else if (name == "events")
		{
			return m_events;
		}
		else if (name == "uis")
		{
			return m_uis;
		}
		else if (name == "loaders")
		{
			return m_loaders;
		}
		else if (name == "renderer")
		{
			return m_renderer;
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
		else if (name == "particles")
		{
			return m_particles;
		}
		else if (name == "skyboxes")
		{
			return m_skyboxes;
		}

		return NULL;
	}
}
