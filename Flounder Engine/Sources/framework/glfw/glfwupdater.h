#pragma once

#include "../iupdater.h"

#include "../../camera/camera.h"
#include "../../devices/audio.h"
#include "../../devices/display.h"
#include "../../devices/joysticks.h"
#include "../../devices/keyboard.h"
#include "../../devices/mouse.h"
#include "../../events/events.h"
#include "../../loaders/loaders.h"
#include "../../logger/logger.h"
#include "../../models/models.h"
#include "../../renderer/renderer.h"
#include "../../shaders/shaders.h"
#include "../../standards/standards.h"
#include "../../processing/processing.h"
#include "../../tasks/tasks.h"
#include "../../textures/textures.h"

#include "../../maths/delta.h"
#include "../../maths/timer.h"

namespace flounder {
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

		logger *m_logger;
		events *m_events;
		tasks *m_tasks;
		processing *m_processing;
		display *m_display;
		joysticks *m_joysticks;
		keyboard *m_keyboard;
		mouse *m_mouse;
		audio *m_audio;
		standards *m_standards;
		camera *m_camera;
		shaders *m_shaders;
		textures *m_textures;
		models *m_models;
		loaders *m_loaders;
		renderer *m_renderer;
	public:
		glfwupdater();

		~glfwupdater();

		void create() override;

		void init() override;

		void update() override;

		imodule *getInstance(std::string name) override;

		inline double getTimeOffset() override { return m_timeOffset; };

		inline void setTimeOffset(double timeOffset) override { m_timeOffset = timeOffset; };

		inline double getDelta() override { return m_deltaUpdate->getChange(); };

		inline double getDeltaRender() override { return m_deltaRender->getChange(); };

		inline void setFpsLimit(double fpsLimit) override { this->m_timerRender->setInterval(1.0 / fpsLimit);  };

		inline double getTimeSec() override { return (glfwGetTime() - m_startTime) + m_timeOffset; };

		inline double getTimeMs() override { return getTimeSec() * 1000.0; };
	};
}
