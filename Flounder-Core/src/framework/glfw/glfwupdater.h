#pragma once

#include "../../camera/camera.h"
#include "../../devices/audio.h"
#include "../../devices/display.h"
#include "../../devices/joysticks.h"
#include "../../devices/keyboard.h"
#include "../../devices/mouse.h"
#include "../../entities/entities.h"
#include "../../events/events.h"
#include "../../loaders/loaders.h"
#include "../../maths/delta.h"
#include "../../maths/timer.h"
#include "../../processing/processing.h"
#include "../../renderer/renderer.h"
#include "../../standards/standards.h"
#include "../../tasks/tasks.h"
#include "../../uis/uis.h"
#include "../../particles/particles.h"
#include "../../skyboxes/skyboxes.h"

#include "../iupdater.h"

namespace flounder
{
	/// <summary>
	/// The default GLFW updater for the framework.
	/// </summary>
	class glfwupdater : public iupdater
	{
	private:
		double m_startTime;
		double m_timeOffset;
		delta *m_deltaUpdate;
		delta *m_deltaRender;
		timer *m_timerUpdate;
		timer *m_timerRender;

		audio *m_audio;
		camera *m_camera;
		display *m_display;
		events *m_events;
		joysticks *m_joysticks;
		keyboard *m_keyboard;
		loaders *m_loaders;
		mouse *m_mouse;
		entities *m_entities;
		processing *m_processing;
		renderer *m_renderer;
		standards *m_standards;
		tasks *m_tasks;
		uis *m_uis;
		particles *m_particles;
		skyboxes *m_skyboxes;
	public:
		glfwupdater();

		~glfwupdater();

		void create() override;

		void update() override;

		imodule *getInstance(const std::string &name) override;

		inline double getTimeOffset() override { return m_timeOffset; };

		inline void setTimeOffset(const double &timeOffset) override { m_timeOffset = timeOffset; };

		inline double getDelta() override { return m_deltaUpdate->getChange(); };

		inline double getDeltaRender() override { return m_deltaRender->getChange(); };

		inline double getTimeSec() override { return (glfwGetTime() - m_startTime) + m_timeOffset; };

		inline double getTimeMs() override { return getTimeSec() * 1000.0; };
	};
}
