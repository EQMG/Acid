#pragma once

#include <chrono>
#include <GLFW/glfw3.h>

#include "framework.h"
#include "maths.h"
#include "delta.h"
#include "timer.h"

class updaterglfw : public updater
{
private:
	double startTime;

	float timeOffset;
	delta *deltaUpdate;
	delta *deltaRender;
	timer *timerUpdate;
	timer *timerRender;

public:
	updaterglfw();
	~updaterglfw();

	void run() override;

	void dispose() override;

	float getTimeOffset() override;

	void setTimeOffset(float) override;

	float getDelta() override;

	float getDeltaRender() override;

	void setFpsLimit(float fpsLimit) override;

	float getTimeSec() override;

	float getTimeMs() override;
};

