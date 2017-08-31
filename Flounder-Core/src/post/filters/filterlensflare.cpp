#include "filterlensflare.hpp"

namespace Flounder
{
	filterlensflare::filterlensflare() :
		ipostfilter("filterLensflare", "res/shaders/filters/lensflare.frag.spv"),
		m_sunPosition(new vector3()),
		m_sunHeight(0.0f)
	{
	}

	filterlensflare::~filterlensflare()
	{
		delete m_sunPosition;
	}

	void filterlensflare::storeValues()
	{
#if 0
		m_shader->loadUniform3f("sunPosition", *m_sunPosition);
		m_shader->loadUniform1f("worldHeight", m_sunHeight);
		m_shader->loadUniform2f("displaySize", static_cast<float>(display::get()->getWidth()), static_cast<float>(display::get()->getHeight()));
#endif
	}

	void filterlensflare::setSunPosition(const vector3 &sunPosition)
	{
		ICamera *camera = Camera::Get()->GetCamera();
		matrix4x4::worldToScreenSpace(sunPosition, *camera->GetViewMatrix(), *camera->GetProjectionMatrix(), m_sunPosition);
	}
}
