#include "Delta.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
	Delta::Delta() :
		m_currentFrameTime(Time::Zero),
		m_lastFrameTime(Time::Zero),
		m_change(Time::Zero),
		m_time(Time::Zero)
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
