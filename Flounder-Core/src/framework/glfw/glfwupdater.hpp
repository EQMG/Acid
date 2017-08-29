#pragma once

#include "../../camera/camera.hpp"
#include "../../devices/audio.hpp"
#include "../../devices/display.hpp"
#include "../../devices/joysticks.hpp"
#include "../../devices/keyboard.hpp"
#include "../../devices/mouse.hpp"
#include "../../events/events.hpp"
#include "../../maths/delta.hpp"
#include "../../maths/timer.hpp"
#include "../../processing/processing.hpp"
#include "../../renderer/renderer.hpp"
#include "../../shadows/shadows.hpp"
#include "../../standards/standards.hpp"
#include "../../tasks/tasks.hpp"
#include "../../terrains/terrains.hpp"
#include "../../uis/uis.hpp"
#include "../../particles/particles.hpp"
#include "../../skyboxes/skyboxes.hpp"
#include "../../waters/waters.hpp"
#include "../../worlds/worlds.hpp"

#include "../iupdater.hpp"

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
