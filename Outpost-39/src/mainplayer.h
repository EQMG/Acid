#pragma once

#include <flounder.h>

using namespace flounder;

class mainplayer :
	public iplayer
{
private:
	vector3 *m_position;
	vector3 *m_rotation;
public:
	mainplayer();

	~mainplayer();

	void update() override;

	inline vector3 *getPosition() const override { return m_position; }

	inline vector3 *getRotation() const override { return m_rotation; }
};
