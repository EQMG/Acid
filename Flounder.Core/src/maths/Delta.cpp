#include "delta.hpp"

namespace Flounder
{
	Delta::Delta() :
		m_currentFrameTime(0.0f),
		m_lastFrameTime(0.0f),
		m_change(0.0f),
		m_time(0.0f)
	{
	}

	Delta::~Delta()
	{
	}

	void Delta::Update()
	{
		m_currentFrameTime = Engine::Get()->GetTime();
		m_change = m_currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = m_currentFrameTime;
		m_time += m_change;
	}
}
