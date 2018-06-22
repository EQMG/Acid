#include "MainUpdater.hpp"

#include <Engine/Engine.hpp>
#include <Maths/Maths.hpp>

namespace test
{
	MainUpdater::MainUpdater() :
		IUpdater(),
		m_deltaUpdate(Delta()),
		m_deltaRender(Delta()),
		m_timerUpdate(Timer(1.0f / 66.0f)),
		m_timerRender(Timer(1.0f / -1.0f))
	{
	}

	MainUpdater::~MainUpdater()
	{
	}

	void MainUpdater::Update(const ModuleRegister &moduleRegister)
	{
		m_timerRender.SetInterval(1.0f / Engine::Get()->GetFpsLimit());

		// Always-Update.
		moduleRegister.RunUpdate(UPDATE_ALWAYS);

		if (m_timerUpdate.IsPassedTime())
		{
			// Resets the timer.
			m_timerUpdate.ResetStartTime();

			// Pre-Update.
			moduleRegister.RunUpdate(UPDATE_PRE);

			// Update.
			moduleRegister.RunUpdate(UPDATE_NORMAL);

			// Post-Update.
			moduleRegister.RunUpdate(UPDATE_POST);

			// Updates the engines delta.
			m_deltaUpdate.Update();
		}

		// Prioritize updates over rendering.
		if (!Maths::AlmostEqual(m_timerUpdate.GetInterval(), m_deltaUpdate.GetChange(), 5.0f))
		{
			return;
		}

		// Renders when needed.
		if (m_timerRender.IsPassedTime() || Engine::Get()->GetFpsLimit() <= 0.0f)
		{
			// Resets the timer.
			m_timerRender.ResetStartTime();

			// Render
			moduleRegister.RunUpdate(UPDATE_RENDER);

			// Updates the render delta, and render time extension.
			m_deltaRender.Update();
		}
	}
}
