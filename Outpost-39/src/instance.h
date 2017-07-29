#pragma once

#include <flounder.h>

using namespace flounder;

class instance :
	public istandard
{
	class playerai
	{
	public:
		entity *m_entity;
		std::string m_name;
		bool m_trackCentre;
		float m_bedXPos;
		bool m_sleeping;
		bool m_adventuting;
		bool m_dead;

		playerai(texture *texture, const std::string &name);

		~playerai();

		void traceX(const float &x);

		void goToCentre() { m_trackCentre = true; }

		bool atCentre() { return maths::almostEqual(m_entity->getPosition()->m_x, 0.5f, 0.007f); }

		void setBed(const float &x)
		{
			m_bedXPos = x;
			m_sleeping = true;
		}

		bool atBed() { return maths::almostEqual(m_entity->getPosition()->m_x, m_bedXPos, 0.007f); }

		void setLevel(const int &level) { m_entity->getPosition()->m_y = (level * 0.15f) + 0.2f; }

		int getLevel() { return (m_entity->getPosition()->m_y - 0.2f) / 0.15f; }

		bool isSleeping() { return m_sleeping; }

		float getBedTarget() { return m_bedXPos; }
	};

private:
	ibutton *m_buttonFullscreen;
	ibutton *m_buttonWireframe;
	ibutton *m_buttonExit;

	float m_time;
	entity *m_terrainDay;
	entity *m_terrainNight;
	entity *m_dome;
	std::vector<playerai*> m_ais;
public:
	instance();

	~instance();

	void update() override;
};
