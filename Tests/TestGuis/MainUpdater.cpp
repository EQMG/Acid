#include "MainUpdater.hpp"

#include <Engine/Engine.hpp>
#include <Maths/Maths.hpp>

namespace test
{
	MainUpdater::MainUpdater() :
		IUpdater(),
		m_deltaUpdate(new Delta()),
		m_deltaRender(new Delta()),
		m_timerUpdate(new Timer(1.0f / 66.0f)),
		m_timerRender(new Timer(1.0f / -1.0f))
	{
	}

	MainUpdater::~MainUpdater()
	{
		delete m_deltaRender;
		delete m_deltaUpdate;

		delete m_timerRender;
		delete m_timerUpdate;
	}

	void MainUpdater::Update(ModuleRegister *moduleRegister)
	{
		m_timerRender->SetInterval(1.0f / Engine::Get()->GetFpsLimit());

		// Always-Update.
		moduleRegister->RunUpdate(ModuleUpdate::UPDATE_ALWAYS);

		if (m_timerUpdate->IsPassedTime())
		{
			// Resets the timer.
			m_timerUpdate->ResetStartTime();

			// Pre-Update.
			moduleRegister->RunUpdate(ModuleUpdate::UPDATE_PRE);

			// Update.
			moduleRegister->RunUpdate(ModuleUpdate::UPDATE_NORMAL);

			// Post-Update.
			moduleRegister->RunUpdate(ModuleUpdate::UPDATE_POST);

			// Updates the engines delta.
			m_deltaUpdate->Update();
		}

		// Prioritize updates over rendering.
		if (!Maths::AlmostEqual(m_timerUpdate->GetInterval(), m_deltaUpdate->GetChange(), 5.0f))
		{
			return;
		}

		// Renders when needed.
		if (m_timerRender->IsPassedTime() || Engine::Get()->GetFpsLimit() <= 0.0f)
		{
			// Resets the timer.
			m_timerRender->ResetStartTime();

			// Render
			moduleRegister->RunUpdate(ModuleUpdate::UPDATE_RENDER);

			// Updates the render delta, and render time extension.
			m_deltaRender->Update();
		}
	}
}
