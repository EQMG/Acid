#pragma once

#include <flounder.hpp>

using namespace Flounder;

class mainplayer :
	public IPlayer
{
private:
	static const float RUN_SPEED;
	static const float STRAFE_SPEED;
	static const float BOOST_MUL;
	static const float JUMP_POWER;
	static const float FLY_SPEED;

	vector3 *m_position;
	vector3 *m_rotation;

	float m_currentSpeed;
	float m_currentStrafeSpeed;
	float m_currentUpwardSpeed;
	iaxis *m_inputForward;
	iaxis *m_inputStrafe;
	iaxis *m_inputNoclip;
	ibutton *m_inputBoost;
	ibutton *m_inputJump;

	vector3 *m_amountMove;
	vector3 *m_amountRotate;

	bool m_paused;
public:
	mainplayer();

	~mainplayer();

	void Update() override;

	inline vector3 *GetPosition() const override { return m_position; }

	inline vector3 *GetRotation() const override { return m_rotation; }
};
