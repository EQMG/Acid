#include "ModuleUpdater.hpp"

#include "Engine/Engine.hpp"
#include "Maths/Maths.hpp"

namespace acid
{
	ModuleUpdater::ModuleUpdater() :
		m_deltaUpdate(Delta()),
		m_deltaRender(Delta()),
		m_timerUpdate(Timer(Time::Seconds(1.0f / 68.0f))),
		m_timerRender(Timer(Time::Seconds(1.0f / -1.0f)))
	{
	}

	void ModuleUpdater::Update(ModuleManager &moduleManager)
	{
		m_timerRender.SetInterval(Time::Seconds(1.0f / Engine::Get()->GetFpsLimit()));

		// Always-Update.
		moduleManager.RunUpdate(MODULE_UPDATE_ALWAYS);

		if (m_timerUpdate.IsPassedTime())
		{
			// Resets the timer.
			m_timerUpdate.ResetStartTime();

			// Updates the engines delta.
			m_deltaUpdate.Update();

			// Pre-Update.
			moduleManager.RunUpdate(MODULE_UPDATE_PRE);

			// Update.
			moduleManager.RunUpdate(MODULE_UPDATE_NORMAL);

			// Post-Update.
			moduleManager.RunUpdate(MODULE_UPDATE_POST);
		}

		// Prioritize updates over rendering.
		if (!Maths::AlmostEqual(m_timerUpdate.GetInterval().AsSeconds(), m_deltaUpdate.GetChange().AsSeconds(), 0.8f))
		{
			return;
		}

		// Renders when needed.
		if (m_timerRender.IsPassedTime())
		{
			// Resets the timer.
			m_timerRender.ResetStartTime();

			// Updates the render delta, and render time extension.
			m_deltaRender.Update();

			// Render
			moduleManager.RunUpdate(MODULE_UPDATE_RENDER);
		}
	}
}
