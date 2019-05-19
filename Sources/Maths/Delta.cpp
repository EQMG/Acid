#include "Delta.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
void Delta::Update()
{
	m_currentFrameTime = Time::Now();
	m_change = m_currentFrameTime - m_lastFrameTime;
	m_lastFrameTime = m_currentFrameTime;
	m_time += m_change;
}
}
