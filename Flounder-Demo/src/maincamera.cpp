#include "maincamera.hpp"

const float maincamera::NEAR_PLANE = 0.1f;
const float maincamera::FAR_PLANE = 2048.1f;
const float maincamera::FIELD_OF_VIEW = 45.0f;

const float maincamera::ROTATE_AGILITY = 20.0f;
const float maincamera::PITCH_AGILITY = 20.0f;

const float maincamera::INFLUENCE_OF_JOYSTICK_DY = 4.5f;
const float maincamera::INFLUENCE_OF_JOYSTICK_DX = 4.5f;

const float maincamera::INFLUENCE_OF_MOUSE_DY = 10000.0f;
const float maincamera::INFLUENCE_OF_MOUSE_DX = 10000.0f;

const float maincamera::MAX_HORIZONTAL_CHANGE = 30.0f;
const float maincamera::MAX_VERTICAL_CHANGE = 30.0f;

const float maincamera::MAX_ANGLE_OF_ELEVATION = 85.0f;
const float maincamera::MIN_ANGLE_OF_ELEVATION = -85.0f;

maincamera::maincamera() :
	m_position(new vector3()),
	m_rotation(new vector3()),
	m_viewMatrix(new matrix4x4()),
	m_projectionMatrix(new matrix4x4()),
	m_viewFrustum(new frustum()),
	m_viewRay(new ray(false, vector2(0.5f, 0.5f))),
	m_angleOfElevation(25.0f),
	m_angleAroundPlayer(0.0f),
	m_targetPosition(new vector3()),
	m_targetRotation(new vector3()),
	m_targetElevation(m_angleOfElevation),
	m_targetRotationAngle(m_angleAroundPlayer),
	m_sensitivity(0.9f),
	m_reangleButton(GLFW_MOUSE_BUTTON_LEFT),
	m_joystickVertical(new axisjoystick(0, 1, 3)),
	m_joystickHorizontal(new axisjoystick(0, 1, 2)),
	m_joystickZoom(new buttonjoystick(0, 1, 9)),
	m_paused(false)
{
}

maincamera::~maincamera()
{
	delete m_position;
	delete m_rotation;

	delete m_viewMatrix;
	delete m_projectionMatrix;

	delete m_viewFrustum;
	delete m_viewRay;

	delete m_targetPosition;
	delete m_targetRotation;

	delete m_joystickVertical;
	delete m_joystickHorizontal;
	delete m_joystickZoom;
}

void maincamera::Update(IPlayer *player)
{
	float delta = __min(1.0f / 60.0f, Engine::Get()->GetDelta());

	if (uis::get() != nullptr && uis::get()->getManager() != nullptr)
	{
		m_paused = uis::get()->getManager()->isGamePaused();
	}

	calculateHorizontalAngle();
	calculateVerticalAngle();

	if (player != nullptr)
	{
		m_targetPosition->set(*player->GetPosition());
		m_targetRotation->set(*player->GetRotation());
	}

	updateHorizontalAngle(delta);
	updatePitchAngle(delta);
	updatePosition();

	matrix4x4::viewMatrix(*m_position, *m_rotation, m_viewMatrix);
	matrix4x4::perspectiveMatrix(GetFov(), static_cast<float>(display::get()->getAspectRatio()), GetNearPlane(), GetFarPlane(), m_projectionMatrix);

	m_viewFrustum->update(*m_projectionMatrix, *m_viewMatrix);
	m_viewRay->update(*m_position, vector2(static_cast<float>(mouse::get()->getPositionX()), static_cast<float>(mouse::get()->getPositionY())), *m_viewMatrix, *m_projectionMatrix);
}

void maincamera::calculateHorizontalAngle()
{
	float angleChange = 0.0f;

	if (!m_paused)
	{
		if (maths::deadband(0.05f, m_joystickHorizontal->getAmount()) != 0.0f && !m_joystickZoom->isDown())
		{
			angleChange = m_joystickHorizontal->getAmount() * INFLUENCE_OF_JOYSTICK_DX * m_sensitivity;
		}
		else
		{
			if (mouse::get()->isCursorDisabled() || mouse::get()->getButton(m_reangleButton))
			{
				angleChange = -mouse::get()->getDeltaX() * INFLUENCE_OF_MOUSE_DX * m_sensitivity;
			}
		}
	}

	if (angleChange > MAX_HORIZONTAL_CHANGE)
	{
		angleChange = MAX_HORIZONTAL_CHANGE;
	}
	else if (angleChange < -MAX_HORIZONTAL_CHANGE)
	{
		angleChange = -MAX_HORIZONTAL_CHANGE;
	}

	m_targetRotationAngle -= angleChange;

	if (m_targetRotationAngle >= DEGREES_IN_HALF_CIRCLE)
	{
		m_targetRotationAngle -= DEGREES_IN_CIRCLE;
	}
	else if (m_targetRotationAngle <= -DEGREES_IN_HALF_CIRCLE)
	{
		m_targetRotationAngle += DEGREES_IN_CIRCLE;
	}
}

void maincamera::calculateVerticalAngle()
{
	float angleChange = 0.0f;

	if (!m_paused)
	{
		if (maths::deadband(0.05f, m_joystickVertical->getAmount()) != 0.0f && !m_joystickZoom->isDown())
		{
			angleChange = m_joystickVertical->getAmount() * INFLUENCE_OF_JOYSTICK_DY * m_sensitivity;
		}
		else
		{
			if (mouse::get()->isCursorDisabled() || mouse::get()->getButton(m_reangleButton))
			{
				angleChange = mouse::get()->getDeltaY() * INFLUENCE_OF_MOUSE_DY * m_sensitivity;
			}
		}
	}

	if (angleChange > MAX_VERTICAL_CHANGE)
	{
		angleChange = MAX_VERTICAL_CHANGE;
	}
	else if (angleChange < -MAX_VERTICAL_CHANGE)
	{
		angleChange = -MAX_VERTICAL_CHANGE;
	}

	m_targetElevation -= angleChange;

	if (m_targetElevation >= MAX_ANGLE_OF_ELEVATION)
	{
		m_targetElevation = MAX_ANGLE_OF_ELEVATION;
	}
	else if (m_targetElevation <= MIN_ANGLE_OF_ELEVATION)
	{
		m_targetElevation = MIN_ANGLE_OF_ELEVATION;
	}
}

void maincamera::updateHorizontalAngle(const float &delta)
{
	float offset = m_targetRotationAngle - m_angleAroundPlayer;

	if (fabs(offset) > DEGREES_IN_HALF_CIRCLE)
	{
		if (offset < 0.0f)
		{
			offset = m_targetRotationAngle + DEGREES_IN_CIRCLE - m_angleAroundPlayer;
		}
		else
		{
			offset = m_targetRotationAngle - DEGREES_IN_CIRCLE - m_angleAroundPlayer;
		}
	}

	m_angleAroundPlayer += offset * delta * ROTATE_AGILITY;

	if (m_angleAroundPlayer >= DEGREES_IN_HALF_CIRCLE)
	{
		m_angleAroundPlayer -= DEGREES_IN_CIRCLE;
	}
	else if (m_angleAroundPlayer <= -DEGREES_IN_HALF_CIRCLE)
	{
		m_angleAroundPlayer += DEGREES_IN_CIRCLE;
	}
}

void maincamera::updatePitchAngle(const float &delta)
{
	float offset = m_targetElevation - m_angleOfElevation;

	if (fabs(offset) > DEGREES_IN_HALF_CIRCLE)
	{
		if (offset < 0.0f)
		{
			offset = m_targetElevation + DEGREES_IN_CIRCLE - m_angleOfElevation;
		}
		else
		{
			offset = m_targetElevation - DEGREES_IN_CIRCLE - m_angleOfElevation;
		}
	}

	m_angleOfElevation += offset * delta * PITCH_AGILITY;

	if (m_angleOfElevation >= DEGREES_IN_HALF_CIRCLE)
	{
		m_angleOfElevation -= DEGREES_IN_CIRCLE;
	}
	else if (m_angleOfElevation <= -DEGREES_IN_HALF_CIRCLE)
	{
		m_angleOfElevation += DEGREES_IN_CIRCLE;
	}
}

void maincamera::updatePosition()
{
	m_position->m_x = m_targetPosition->m_x;
	m_position->m_y = m_targetPosition->m_y;
	m_position->m_z = m_targetPosition->m_z;

	m_rotation->m_x = m_angleOfElevation - m_targetRotation->m_z;
	m_rotation->m_y = m_angleAroundPlayer + m_targetRotation->m_y + DEGREES_IN_HALF_CIRCLE;
	m_rotation->m_z = 0.0f;
}

void maincamera::ReflectView(const float &waterHeight)
{
	m_position->m_y -= 2.0f * (m_position->m_y - waterHeight);
	m_rotation->m_x = -m_rotation->m_x;
	matrix4x4::viewMatrix(*m_position, *m_rotation, m_viewMatrix);
}
