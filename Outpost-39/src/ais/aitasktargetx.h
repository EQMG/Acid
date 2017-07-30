#pragma once

#include "iaitask.h"

using namespace flounder;

class aitasktargetx :
	public iaitask
{
private:
	static const float SPEED;
	static const float ERROR;

	float m_targetX;
public:
	aitasktargetx(entity* object, const float &targetX);

	~aitasktargetx();

	void start() override;

	void update() override;

	bool isComplete() override;
};
