#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class FpsPlayer :
	public IPlayer
{
private:
	static const float RUN_SPEED;
	static const float STRAFE_SPEED;
	static const float VERTICAL_SPEED;
	static const float BOOST_SCALE;
	static const float SLOW_SCALE;

	Vector3 *m_position;
	Vector3 *m_rotation;

	float m_currentSpeed;
	float m_currentStrafeSpeed;
	float m_currentUpwardSpeed;
	IAxis *m_inputForward;
	IAxis *m_inputStrafe;
	IAxis *m_inputVertical;
	IButton *m_inputBoost;
	IButton *m_inputSlow;

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
