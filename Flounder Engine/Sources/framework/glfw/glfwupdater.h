#pragma once

#include "../updater.h"

#include "../../camera/camera.h"
#include "../../devices/audio.h"
#include "../../devices/display.h"
#include "../../devices/joysticks.h"
#include "../../devices/keyboard.h"
#include "../../devices/mouse.h"
#include "../../logger/logger.h"
#include "../../tasks/tasks.h"

#include "../../maths/delta.h"
#include "../../maths/timer.h"

namespace flounder {
	class glfwupdater : public updater
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
		tasks *m_tasks;
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

		inline double getTimeOffset() override { return m_timeOffset; };

		inline void setTimeOffset(double timeOffset) override { m_timeOffset = timeOffset; };

		inline double getDelta() override { return m_deltaUpdate->getChange(); };

		inline double getDeltaRender() override { return m_deltaRender->getChange(); };

		inline void setFpsLimit(double fpsLimit) override { this->m_timerRender->setInterval(1.0 / fpsLimit);  };

		inline double getTimeSec() override { return (glfwGetTime() - m_startTime) + m_timeOffset; };

		inline double getTimeMs() override { return getTimeSec() * 1000.0; };
	};
}
