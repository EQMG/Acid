#pragma once

#include "../../camera/camera.h"
#include "../../devices/audio.h"
#include "../../devices/display.h"
#include "../../devices/joysticks.h"
#include "../../devices/keyboard.h"
#include "../../devices/mouse.h"
#include "../../events/events.h"
#include "../../loaders/loaders.h"
#include "../../maths/delta.h"
#include "../../maths/timer.h"
#include "../../processing/processing.h"
#include "../../renderer/renderer.h"
#include "../../shadows/shadows.h"
#include "../../standards/standards.h"
#include "../../tasks/tasks.h"
#include "../../terrains/terrains.h"
#include "../../uis/uis.h"
#include "../../particles/particles.h"
#include "../../skyboxes/skyboxes.h"
#include "../../waters/waters.h"
#include "../../worlds/worlds.h"

#include "../iupdater.h"

namespace flounder
{
	/// <summary>
	/// The default GLFW updater for the framework.
	/// </summary>
	class glfwupdater : public iupdater
	{
	private:
		float m_startTime;
		float m_timeOffset;
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
		processing *m_processing;
		renderer *m_renderer;
		shadows *m_shadows;
		standards *m_standards;
		tasks *m_tasks;
		uis *m_uis;
		particles *m_particles;
		skyboxes *m_skyboxes;
		terrains *m_terrains;
		waters *m_waters;
		worlds *m_worlds;
	public:
		glfwupdater();

		~glfwupdater();

		void create() override;

		void update() override;

		imodule *getInstance(const std::string &name) override;

		inline float getTimeOffset() override { return m_timeOffset; };

		inline void setTimeOffset(const float &timeOffset) override { m_timeOffset = timeOffset; };

		inline float getDelta() override { return m_deltaUpdate->getChange(); };

		inline float getDeltaRender() override { return m_deltaRender->getChange(); };

		inline float getTimeSec() override { return (static_cast<float>(glfwGetTime()) - m_startTime) + m_timeOffset; };

		inline float getTimeMs() override { return getTimeSec() * 1000.0f; };
	};
}
