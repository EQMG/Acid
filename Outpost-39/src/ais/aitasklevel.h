#pragma once

#include "iaitask.h"

using namespace flounder;

class aitasklevel :
	public iaitask
{
private:
	float m_level;
public:
	aitasklevel(entity* object, const int &level);

	~aitasklevel();

	void update() override;

	bool isComplete() override;
};
