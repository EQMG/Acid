#pragma once

#include "iaitask.h"

using namespace flounder;

class aitaskadventure :
	public iaitask
{
private:
	static const float LENGTH;

	timer *m_timer;
	bool complete;
public:
	aitaskadventure(entity* object);

	~aitaskadventure();

	void start() override;

	void update() override;

	bool isComplete() override;
};
