#include "ModuleUpdater.hpp"

#include "Engine/Engine.hpp"
#include "Maths/Maths.hpp"

namespace acid
{
ModuleUpdater::ModuleUpdater() : m_timerUpdate(Time::Seconds(1.0f / 68.0f)), m_timerRender(Time::Seconds(1.0f / -1.0f)), m_ups(), m_fps() {}

void ModuleUpdater::Update(ModuleManager& moduleManager)
{
	m_timerRender.SetInterval(Time::Seconds(1.0f / Engine::Get()->GetFpsLimit()));

	// Always-Update.
	moduleManager.RunUpdate(Module::Stage::Always);

	if(m_timerUpdate.IsPassedTime())
		{
			// Resets the timer.
			m_timerUpdate.ResetStartTime();
			m_ups.Update(Engine::GetTime().AsSeconds());

			// Pre-Update.
			moduleManager.RunUpdate(Module::Stage::Pre);

			// Update.
			moduleManager.RunUpdate(Module::Stage::Normal);

			// Post-Update.
			moduleManager.RunUpdate(Module::Stage::Post);

			// Updates the engines delta.
			m_deltaUpdate.Update();
		}

	// Prioritize
	// updates
	// over
	// rendering.
	if(!Maths::AlmostEqual(m_timerUpdate.GetInterval().AsSeconds(), m_deltaUpdate.GetChange().AsSeconds(), 0.8f))
		{
			return;
		}

	// Renders
	// when
	// needed.
	if(m_timerRender.IsPassedTime())
		{
			// Resets the timer.
			m_timerRender.ResetStartTime();
			m_fps.Update(Engine::GetTime().AsSeconds());

			// Render
			moduleManager.RunUpdate(Module::Stage::Render);

			// Updates the render delta, and render time extension.
			m_deltaRender.Update();
		}
}
}
