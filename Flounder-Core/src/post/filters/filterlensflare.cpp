#include "filterlensflare.h"

namespace flounder
{
	filterlensflare::filterlensflare() :
		ipostfilter("filterLensflare", "res/shaders/filters/lensflareFragment.glsl")
	{
		m_sunPosition = new vector3();
		m_sunHeight = 0.0f;
	}

	filterlensflare::~filterlensflare()
	{
		delete m_sunPosition;
	}

	void filterlensflare::storeValues()
	{
		m_shader->loadUniform("sunPosition", *m_sunPosition);
		m_shader->loadUniform("worldHeight", m_sunHeight);
		m_shader->loadUniform("displaySize", static_cast<float>(display::get()->getWidth()), static_cast<float>(display::get()->getHeight()));
	}

	void filterlensflare::setSunPosition(const vector3 &sunPosition)
	{
		icamera *camera = camera::get()->getCamera();
		matrix4x4::worldToScreenSpace(sunPosition, *camera->getViewMatrix(), *camera->getProjectionMatrix(), m_sunPosition);
	}
}
