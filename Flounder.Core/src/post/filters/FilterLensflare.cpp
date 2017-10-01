#include "FilterLensflare.hpp"

#include "../../camera/Camera.hpp"
#include "../../devices/Display.hpp"

namespace Flounder
{
	FilterLensflare::FilterLensflare() :
		IPostFilter("filterLensflare", "res/shaders/filters/lensflare.frag.spv"),
		m_sunPosition(new Vector3()),
		m_sunHeight(0.0f)
	{
	}

	FilterLensflare::~FilterLensflare()
	{
		delete m_sunPosition;
	}

	void FilterLensflare::StoreValues()
	{
#if 0
		m_shader->loadUniform3f("sunPosition", *m_sunPosition);
		m_shader->loadUniform1f("worldHeight", m_sunHeight);
		m_shader->loadUniform2f("displaySize", static_cast<float>(Display::Get()->getWidth()), static_cast<float>(Display::Get()->getHeight()));
#endif
	}

	void FilterLensflare::SetSunPosition(const Vector3 &sunPosition) const
	{
		ICamera *camera = Camera::Get()->GetCamera();
		Matrix4::WorldToScreenSpace(sunPosition, *camera->GetViewMatrix(), *camera->GetProjectionMatrix(), m_sunPosition);
	}
}
