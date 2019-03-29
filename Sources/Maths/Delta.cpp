#include "Delta.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
void Delta::Update()
{
	m_currentFrameTime = Engine::GetTime();
	m_change = m_currentFrameTime - m_lastFrameTime;
	m_lastFrameTime = m_currentFrameTime;
	m_time += m_change;
}
}
