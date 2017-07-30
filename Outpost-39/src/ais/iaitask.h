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
	iaitask(entity* object)
	{
		m_entity = object;
	}

	virtual ~iaitask()
	{
	}

	virtual void update() = 0;

	virtual bool isComplete() = 0;
};