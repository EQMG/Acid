#pragma once

#include <map>

#include "../../devices/Display.hpp"

#include "../../maths/delta.hpp"
#include "../../maths/timer.hpp"

#include "../../engine/IUpdater.hpp"

namespace Flounder
{
	/// <summary>
	/// The default GLFW updater for the engine.
	/// </summary>
	class GlfwUpdater :
		public IUpdater
	{
	private:
		float m_startTime;
		float m_timeOffset;
		delta *m_deltaUpdate;
		delta *m_deltaRender;
		timer *m_timerUpdate;
		timer *m_timerRender;

		std::multimap<float, std::pair<std::string, IModule*>> *m_modules;
	public:
		GlfwUpdater();

		~GlfwUpdater();

		void Create() override;

		void Update() override;

		void AddModule(ModuleUpdate typeUpdate, std::string moduleName, IModule *module) override;

		IModule *GetModule(const std::string &name) override;

		float GetTimeOffset() override { return m_timeOffset; };

		void SetTimeOffset(const float &timeOffset) override { m_timeOffset = timeOffset; };

		float GetDelta() override { return m_deltaUpdate->getChange(); };

		float GetDeltaRender() override { return m_deltaRender->getChange(); };

		float GetTime() override { return (static_cast<float>(glfwGetTime()) - m_startTime) + m_timeOffset; };

		float GetTimeMs() override { return GetTime() * 1000.0f; };
	private:
		void RunUpdate(ModuleUpdate typeUpdate);
	};
}
