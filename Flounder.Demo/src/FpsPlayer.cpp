#include "FpsPlayer.hpp"

const float FpsPlayer::RUN_SPEED = 6.0f;
const float FpsPlayer::STRAFE_SPEED = 4.0f;
const float FpsPlayer::BOOST_MUL = 2.0f;
const float FpsPlayer::JUMP_POWER = 8.0f;
const float FpsPlayer::FLY_SPEED = 10.0f;

FpsPlayer::FpsPlayer() :
	IPlayer(),
	m_position(new Vector3()),
	m_rotation(new Vector3()),
	m_currentSpeed(0.0f),
	m_currentStrafeSpeed(0.0f),
	m_currentUpwardSpeed(0.0f),
	m_inputForward(new AxisCompound(2, new AxisButton(new ButtonKeyboard(2, GLFW_KEY_S, GLFW_KEY_DOWN), new ButtonKeyboard(2, GLFW_KEY_W, GLFW_KEY_UP)), new AxisJoystick(0, 1))),
	m_inputStrafe(new AxisCompound(2, new AxisButton(new ButtonKeyboard(2, GLFW_KEY_A, GLFW_KEY_LEFT), new ButtonKeyboard(2, GLFW_KEY_D, GLFW_KEY_RIGHT)), new AxisJoystick(0, 0))),
	m_inputNoclip(new AxisCompound(2, new AxisButton(new ButtonKeyboard(2, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL), new ButtonKeyboard(1, GLFW_KEY_SPACE)), new AxisJoystick(0, 0))),
	m_inputBoost(new ButtonCompound(2, new ButtonKeyboard(2, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT), new ButtonJoystick(0, 1))),
	m_inputJump(new ButtonCompound(2, new ButtonKeyboard(1, GLFW_KEY_SPACE), new ButtonJoystick(0, 0))),
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
	delete m_inputNoclip;
	delete m_inputBoost;
	delete m_inputJump;

	delete m_amountMove;
	delete m_amountRotate;
}

void FpsPlayer::Update()
{
	// Gets the delta and limits the lowest UPS to 20 (any less and the game is unplayable).
	float delta = Maths::Min(Engine::Get()->GetDelta(), 1.0f / 20.0f);

	if (uis::get() != nullptr && uis::get()->getManager() != nullptr)
	{
		m_paused = uis::get()->getManager()->IsGamePaused();
	}

	if (!m_paused)
	{
		m_currentSpeed = RUN_SPEED * Maths::Deadband(0.05f, m_inputForward->GetAmount());
		m_currentStrafeSpeed = -STRAFE_SPEED * Maths::Deadband(0.05f, m_inputStrafe->GetAmount());

		if (m_inputJump->WasDown() && Maths::Deadband(0.1f, m_currentUpwardSpeed) == 0.0f)
		{
			m_currentUpwardSpeed = JUMP_POWER;
		}
	}
	else
	{
		m_currentSpeed = 0.0f;
		m_currentStrafeSpeed = 0.0f;
	}

	if (!m_paused)
	{
		m_currentSpeed *= 0.5f * FLY_SPEED;
		m_currentStrafeSpeed *= 0.5f * FLY_SPEED;
		m_currentUpwardSpeed = 2.0f * m_inputNoclip->GetAmount() * FLY_SPEED;

		if (m_inputBoost->IsDown())
		{
			m_currentSpeed *= BOOST_MUL;
			m_currentStrafeSpeed *= BOOST_MUL;
			m_currentUpwardSpeed *= BOOST_MUL;
		}
	}

	// Calculates the deltas to the moved distance, and rotations.
	float theta = Maths::Radians(Camera::Get()->GetCamera()->GetRotation()->m_y);
	float dx = -(m_currentSpeed * sin(theta) + m_currentStrafeSpeed * cos(theta)) * delta;
	float dy = m_currentUpwardSpeed * delta;
	float dz = -(m_currentSpeed * cos(theta) - m_currentStrafeSpeed * sin(theta)) * delta;

	Vector3::Add(*m_position, *m_amountMove->Set(dx, dy, dz), m_position);
	Vector3::Add(*m_rotation, *m_amountRotate->Set(0.0f, 0.0f, 0.0f), m_rotation);
}
