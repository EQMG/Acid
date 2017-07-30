#pragma once

#include "../ais/iaitask.h"
#include "../instance.h"

using namespace flounder;

class aitasksleep :
	public iaitask
{
private:
	float m_bedPositionX;
	bool m_sleeping;
public:
	aitasksleep(entity* object);

	~aitasksleep();

	void start() override;

	void update() override;

	bool isComplete() override;
};
