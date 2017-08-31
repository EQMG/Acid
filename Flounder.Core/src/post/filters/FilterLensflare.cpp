#include "filterlensflare.hpp"

namespace Flounder
{
	filterlensflare::filterlensflare() :
		ipostfilter("filterLensflare", "res/shaders/filters/lensflare.frag.spv"),
		m_sunPosition(new Vector3()),
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
		m_shader->loadUniform2f("displaySize", static_cast<float>(Display::Get()->getWidth()), static_cast<float>(Display::Get()->getHeight()));
#endif
	}

	void filterlensflare::setSunPosition(const Vector3 &sunPosition)
	{
		ICamera *camera = Camera::Get()->GetCamera();
		Matrix4::worldToScreenSpace(sunPosition, *camera->GetViewMatrix(), *camera->GetProjectionMatrix(), m_sunPosition);
	}
}
