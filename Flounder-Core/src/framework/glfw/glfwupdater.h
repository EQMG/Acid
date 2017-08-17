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
	enum moduleupdate
	{
		UpdateAlways = 0,
		UpdatePre = 1,
		Update = 2,
		UpdatePost = 3,
		Render = 4
	};

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

		std::multimap<float, std::pair<std::string, imodule*>> *m_modules;
	public:
		glfwupdater();

		~glfwupdater();

		void create() override;

		void update() override;

		void addModule(moduleupdate typeUpdate, std::string moduleName, imodule *object);

		imodule *getInstance(const std::string &name) override;

		inline float getTimeOffset() override { return m_timeOffset; };

		inline void setTimeOffset(const float &timeOffset) override { m_timeOffset = timeOffset; };

		inline float getDelta() override { return m_deltaUpdate->getChange(); };

		inline float getDeltaRender() override { return m_deltaRender->getChange(); };

		inline float getTimeSec() override { return (static_cast<float>(glfwGetTime()) - m_startTime) + m_timeOffset; };

		inline float getTimeMs() override { return getTimeSec() * 1000.0f; };
	private:
		void runUpdate(moduleupdate typeUpdate);
	};
}
