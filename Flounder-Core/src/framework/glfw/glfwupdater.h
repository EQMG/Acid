#pragma once

#include "../../camera/camera.h"
#include "../../devices/audio.h"
#include "../../devices/display.h"
#include "../../devices/joysticks.h"
#include "../../devices/keyboard.h"
#include "../../devices/mouse.h"
#include "../../events/events.h"
#include "../../guis/guis.h"
#include "../../loaders/loaders.h"
#include "../../maths/delta.h"
#include "../../maths/timer.h"
#include "../../particles/particles.h"
#include "../../processing/processing.h"
#include "../../renderer/renderer.h"
#include "../../skybox/skybox.h"
#include "../../standards/standards.h"
#include "../../tasks/tasks.h"

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
		timer *m_timerLog;

		audio *m_audio;
		camera *m_camera;
		display *m_display;
		events *m_events;
		guis *m_guis;
		joysticks *m_joysticks;
		keyboard *m_keyboard;
		loaders *m_loaders;
		mouse *m_mouse;
		particles *m_particles;
		processing *m_processing;
		renderer *m_renderer;
		skybox *m_skybox;
		standards *m_standards;
		tasks *m_tasks;
	public:
		glfwupdater();

		~glfwupdater();

		void create() override;

		void init() override;

		void update() override;

		imodule *getInstance(const std::string &name) override;

		inline double getTimeOffset() override { return m_timeOffset; };

		inline void setTimeOffset(const double &timeOffset) override { m_timeOffset = timeOffset; };

		inline double getDelta() override { return m_deltaUpdate->m_change; };

		inline double getDeltaRender() override { return m_deltaRender->m_change; };

		inline double getTimeSec() override { return (glfwGetTime() - m_startTime) + m_timeOffset; };

		inline double getTimeMs() override { return getTimeSec() * 1000.0; };
	};
}
