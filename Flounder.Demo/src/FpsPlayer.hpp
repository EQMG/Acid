#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class FpsPlayer :
	public IPlayer
{
private:
	static const float RUN_SPEED;
	static const float STRAFE_SPEED;
	static const float BOOST_MUL;
	static const float JUMP_POWER;
	static const float FLY_SPEED;

	Vector3 *m_position;
	Vector3 *m_rotation;

	float m_currentSpeed;
	float m_currentStrafeSpeed;
	float m_currentUpwardSpeed;
	iaxis *m_inputForward;
	iaxis *m_inputStrafe;
	iaxis *m_inputNoclip;
	ibutton *m_inputBoost;
	ibutton *m_inputJump;

	Vector3 *m_amountMove;
	Vector3 *m_amountRotate;

	bool m_paused;
public:
	FpsPlayer();

	~FpsPlayer();

	void Update() override;

	inline Vector3 *GetPosition() const override { return m_position; }

	inline Vector3 *GetRotation() const override { return m_rotation; }
};
