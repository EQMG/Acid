#include "updaterglfw.h"

updaterglfw::updaterglfw()
{
	// Sets the timing for the updater to run from.
	this->startTime = glfwGetTime();

	// Creates variables to be used for timing updates and renders.
	this->timeOffset = 0.0f;
	this->deltaUpdate = new delta();
	this->deltaRender = new delta();
	this->timerUpdate = new timer(1.0 / 60.0);
	this->timerRender = new timer(1.0 / framework::get()->getFpsLimit());
}

updaterglfw::~updaterglfw()
{
	delete &startTime;

	delete &timeOffset;
	delete deltaUpdate;
	delete deltaRender;
	delete timerUpdate;
	delete timerRender;
}

void updaterglfw::run()
{
	if (framework::get()->isInitialized())
	{
		return;
	}

	// Initializes all modules.
	for (int i = 0; i < framework::get()->getModules().size(); i++) {
		framework::get()->getModules().at(i)->init();
	}

	// Logs initialize times.
	// logger::get()->init("Framework Initialize & Load Time: " + logger::ANSI_RED + (getTimeSec() - startTime) + logger::ANSI_RESET + " seconds!");

	// Sets the framework as initialized.
	framework::get()->setInitialized(true);

	while (framework::get()->isRunning())
	{
		if (framework::get()->isInitialized())
		{
			// Updates the module when needed always.
			for (int i = 0; i < framework::get()->getModules().size(); i++) {
				if (framework::get()->getModules().at(i)->getFlag() == module::ALWAYS) {
					framework::get()->getModules().at(i)->update();
				}
			}

			// Updates when needed.
			if (timerUpdate->isPassedTime())
			{
				// Resets the timer.
				timerUpdate->resetStartTime();

				// Updates the frameworks delta.
				deltaUpdate->update();

				// Updates the modules when needed before the entrance.
				for (int i = 0; i < framework::get()->getModules().size(); i++) {
					if (framework::get()->getModules().at(i)->getFlag() == module::BEFORE) {
						framework::get()->getModules().at(i)->update();
					}
				}

				// Updates the modules when needed after the entrance.
				for (int i = 0; i < framework::get()->getModules().size(); i++) {
					if (framework::get()->getModules().at(i)->getFlag() == module::AFTER) {
						framework::get()->getModules().at(i)->update();
					}
				}
			}

			// Renders when needed.
			if ((timerRender->isPassedTime() || framework::get()->getFpsLimit() == -1 || framework::get()->getFpsLimit() > 1000.0f) && maths::almostEqual(timerUpdate->m_interval, deltaUpdate->m_change, 10.0))
			{
				// Resets the timer.
				timerRender->resetStartTime();

				// Updates the render delta, and render time extension.
				deltaRender->update();

				// Updates the module when needed after the rendering.
				for (int i = 0; i < framework::get()->getModules().size(); i++) {
					framework::get()->getModules().at(i)->render();
				}
			}
		}
	}
}

void updaterglfw::dispose()
{
	if (!framework::get()->isInitialized())
	{
		return;
	}

	// logger::get()->warning("Disposing framework!"); // A new Framework object must be recreated if resetting the framework!

	for (int i = framework::get()->getModules().size(); i >= 0; i++) {
		framework::get()->getModules().at(i)->render();
	}

	framework::get()->getModules().clear();
	framework::get()->setInitialized(false);
}

float updaterglfw::getTimeOffset()
{
	return timeOffset;
}

void updaterglfw::setTimeOffset(float)
{
	this->timeOffset = timeOffset;
}

float updaterglfw::getDelta()
{
	return deltaUpdate->m_change;
}

float updaterglfw::getDeltaRender()
{
	return deltaRender->m_change;
}

void updaterglfw::setFpsLimit(float fpsLimit)
{
	this->timerRender->m_interval = abs(1.0f / fpsLimit);
}

float updaterglfw::getTimeSec()
{
	return glfwGetTime() - startTime + timeOffset;
}

float updaterglfw::getTimeMs()
{
	return getTimeSec() * 1000.0f;
}
