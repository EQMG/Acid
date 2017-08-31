#include "mainplayer.hpp"

const float mainplayer::RUN_SPEED = 6.0f;
const float mainplayer::STRAFE_SPEED = 4.0f;
const float mainplayer::BOOST_MUL = 2.0f;
const float mainplayer::JUMP_POWER = 8.0f;
const float mainplayer::FLY_SPEED = 10.0f;

mainplayer::mainplayer() :
	IPlayer(),
	m_position(new vector3()),
	m_rotation(new vector3()),
	m_currentSpeed(0.0f),
	m_currentStrafeSpeed(0.0f),
	m_currentUpwardSpeed(0.0f),
	m_inputForward(new axiscompound(2, new axisbutton(new buttonkeyboard(2, GLFW_KEY_S, GLFW_KEY_DOWN), new buttonkeyboard(2, GLFW_KEY_W, GLFW_KEY_UP)), new axisjoystick(0, 1))),
	m_inputStrafe(new axiscompound(2, new axisbutton(new buttonkeyboard(2, GLFW_KEY_A, GLFW_KEY_LEFT), new buttonkeyboard(2, GLFW_KEY_D, GLFW_KEY_RIGHT)), new axisjoystick(0, 0))),
	m_inputNoclip(new axiscompound(2, new axisbutton(new buttonkeyboard(2, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL), new buttonkeyboard(1, GLFW_KEY_SPACE)), new axisjoystick(0, 0))),
	m_inputBoost(new buttoncompound(2, new buttonkeyboard(2, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT), new buttonjoystick(0, 1))),
	m_inputJump(new buttoncompound(2, new buttonkeyboard(1, GLFW_KEY_SPACE), new buttonjoystick(0, 0))),
	m_amountMove(new vector3()),
	m_amountRotate(new vector3()),
	m_paused(false)
{
}

mainplayer::~mainplayer()
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

void mainplayer::Update()
{
	// Gets the delta and limits the lowest UPS to 20 (any less and the game is unplayable).
	float delta = __min(Engine::Get()->GetDelta(), 1.0f / 20.0f);

	if (uis::get() != nullptr && uis::get()->getManager() != nullptr)
	{
		m_paused = uis::get()->getManager()->isGamePaused();
	}

	if (!m_paused)
	{
		m_currentSpeed = RUN_SPEED * maths::deadband(0.05f, m_inputForward->getAmount());
		m_currentStrafeSpeed = -STRAFE_SPEED * maths::deadband(0.05f, m_inputStrafe->getAmount());

		if (m_inputJump->wasDown() && maths::deadband(0.1f, m_currentUpwardSpeed) == 0.0f)
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
		m_currentUpwardSpeed = 2.0f * m_inputNoclip->getAmount() * FLY_SPEED;

		if (m_inputBoost->isDown())
		{
			m_currentSpeed *= BOOST_MUL;
			m_currentStrafeSpeed *= BOOST_MUL;
			m_currentUpwardSpeed *= BOOST_MUL;
		}
	}

	// Calculates the deltas to the moved distance, and rotations.
	float theta = __radians(Camera::Get()->GetCamera()->GetRotation()->m_y);
	float dx = -(m_currentSpeed * sin(theta) + m_currentStrafeSpeed * cos(theta)) * delta;
	float dy = m_currentUpwardSpeed * delta;
	float dz = -(m_currentSpeed * cos(theta) - m_currentStrafeSpeed * sin(theta)) * delta;

	vector3::add(*m_position, *m_amountMove->set(dx, dy, dz), m_position);
	vector3::add(*m_rotation, *m_amountRotate->set(0.0f, 0.0f, 0.0f), m_rotation);
}
