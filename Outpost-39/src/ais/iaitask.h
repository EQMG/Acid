#pragma once

#include <flounder.h>

using namespace flounder;

class iaitask
{
protected:
	entity* m_entity;
	float m_lowerHeight = 0.842f;
	float m_levelHeight = 0.134f;

	int positionToLevel(const float &y) 
	{
		return (m_lowerHeight - y) / m_levelHeight;
	}

	float levelToPosition(const int &level)
	{
		return m_lowerHeight - (level * m_levelHeight);
	}
public:
	bool m_started;

	iaitask(entity* object)
	{
		m_entity = object;
		m_started = false;
	}

	virtual ~iaitask()
	{
	}

	virtual void start() = 0;

	virtual void update() = 0;

	virtual bool isComplete() = 0;
};