#pragma once

#include "iaitask.h"

using namespace flounder;

class aitasklevel :
	public iaitask
{
private:
	static const float TIME_DELAY;

	float m_level;
	bool m_instant;
	timer *m_timer;
public:
	aitasklevel(entity* object, const int &level, const bool &instant);

	~aitasklevel();

	void start() override;

	void update() override;

	bool isComplete() override;
};
