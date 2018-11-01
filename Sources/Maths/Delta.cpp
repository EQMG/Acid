#include "Delta.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
	Delta::Delta() :
		m_currentFrameTime(Time::ZERO),
		m_lastFrameTime(Time::ZERO),
		m_change(Time::ZERO),
		m_time(Time::ZERO)
	{
	}

	void Delta::Update()
	{
		m_currentFrameTime = Engine::GetTime();
		m_change = m_currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = m_currentFrameTime;
		m_time += m_change;
	}
}
