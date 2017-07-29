#pragma once

#include <flounder.h>

using namespace flounder;

class aiplayer
{
private:
	entity *m_entity;
	std::string m_name;
	bool m_trackCentre;
	float m_bedXPos;
	bool m_sleeping;
	bool m_adventuting;
	bool m_dead;
public:
	aiplayer(texture *texture, const std::string &name);

	~aiplayer();

	std::string getName() const { return m_name; }

	bool isAtCentre();

	void goToCentre();

	int getLevel();

	void setLevel(const int &level);

	bool isAtBed();

	float getBedTarget() const { return m_bedXPos; }

	void setBedTarget(const float &x);

	bool isSleeping() const { return m_sleeping; }

	bool isAdventuring() const { return m_adventuting; }

	void cancelSleep();
private:
	void traceX(const float &x);
};
