#pragma once

#include "iaitask.h"

using namespace flounder;

class aitaskfade :
	public iaitask
{
private:
	static const float TIME_FADE;

	float m_target;
	bool m_instant;
	driverslide *m_driver;
public:
	aitaskfade(entity* object, const float &target, const bool &instant);

	~aitaskfade();

	void start() override;

	void update() override;

	bool isComplete() override;
};
