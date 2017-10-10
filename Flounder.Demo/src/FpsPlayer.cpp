#include "FpsPlayer.hpp"

namespace Demo
{
	const float FpsPlayer::RUN_SPEED = 5.0f;
	const float FpsPlayer::STRAFE_SPEED = 5.0f;
	const float FpsPlayer::VERTICAL_SPEED = 5.0f;
	const float FpsPlayer::BOOST_SCALE = 5.0f;
	const float FpsPlayer::SLOW_SCALE = 0.2f;

	FpsPlayer::FpsPlayer() :
		IPlayer(),
		m_position(new Vector3()),
		m_rotation(new Vector3()),
		m_currentSpeed(0.0f),
		m_currentStrafeSpeed(0.0f),
		m_currentUpwardSpeed(0.0f),
		m_inputForward(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({ GLFW_KEY_S, GLFW_KEY_DOWN }),
				new ButtonKeyboard({ GLFW_KEY_W, GLFW_KEY_UP })
			),
			new AxisJoystick(0, { 1 })
	})),
		m_inputStrafe(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({ GLFW_KEY_D, GLFW_KEY_RIGHT }),
				new ButtonKeyboard({ GLFW_KEY_A, GLFW_KEY_LEFT })
			),
			new AxisJoystick(0, { 0 })
	})),
		m_inputVertical(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({ GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL }),
				new ButtonKeyboard({ GLFW_KEY_SPACE })
			),
			new AxisJoystick(0, { 0 })
	})),
		m_inputBoost(new ButtonCompound({
			new ButtonKeyboard({ GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT }),
			new ButtonJoystick(0, { 1 })
	})),
		m_inputSlow(new ButtonCompound({
			new ButtonKeyboard({ GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT }),
			new ButtonJoystick(0, { 1 })
	})),
		m_amountMove(new Vector3()),
		m_amountRotate(new Vector3()),
		m_paused(false)
	{
	}

	FpsPlayer::~FpsPlayer()
	{
		delete m_position;
		delete m_rotation;

		delete m_inputForward;
		delete m_inputStrafe;
		delete m_inputVertical;
		delete m_inputBoost;
		delete m_inputSlow;

		delete m_amountMove;
		delete m_amountRotate;
	}

	void FpsPlayer::Update()
	{
		// Gets the delta and limits the lowest UPS to 20 (any less and the game is unplayable).
		const float delta = Maths::Min(Engine::Get()->GetDelta(), 1.0f / 20.0f);

		if (Uis::get() != nullptr && Uis::get()->GetManager() != nullptr)
		{
			m_paused = Uis::get()->GetManager()->IsGamePaused();
		}

		if (!m_paused)
		{
			m_currentSpeed = RUN_SPEED * m_inputForward->GetAmount();
			m_currentStrafeSpeed = STRAFE_SPEED * m_inputStrafe->GetAmount();
			m_currentUpwardSpeed = VERTICAL_SPEED * m_inputVertical->GetAmount();

			if (m_inputBoost->IsDown())
			{
				m_currentSpeed *= BOOST_SCALE;
				m_currentStrafeSpeed *= BOOST_SCALE;
				m_currentUpwardSpeed *= BOOST_SCALE;
			}
			else if (m_inputSlow->IsDown())
			{
				m_currentSpeed *= SLOW_SCALE;
				m_currentStrafeSpeed *= SLOW_SCALE;
				m_currentUpwardSpeed *= SLOW_SCALE;
			}
		}
		else
		{
			m_currentSpeed = 0.0f;
			m_currentStrafeSpeed = 0.0f;
			m_currentUpwardSpeed = 0.0f;
		}

		// Calculates the deltas to the moved distance, and rotations.
		const float theta = Maths::Radians(Camera::Get()->GetCamera()->GetRotation()->m_y);
		const float dx = -(m_currentSpeed * sin(theta) + m_currentStrafeSpeed * cos(theta)) * delta;
		const float dy = m_currentUpwardSpeed * delta;
		const float dz = -(m_currentSpeed * cos(theta) - m_currentStrafeSpeed * sin(theta)) * delta;

		Vector3::Add(*m_position, *m_amountMove->Set(dx, dy, dz), m_position);
		Vector3::Add(*m_rotation, *m_amountRotate->Set(0.0f, 0.0f, 0.0f), m_rotation);

		const float terrainHeight = Terrains::Get()->GetHeight(m_position->m_x, m_position->m_z) + 1.0f;

		if (m_position->m_y < terrainHeight)
		{
			m_position->m_y = terrainHeight;
		}
	}
}
