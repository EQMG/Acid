#include "FilterMotion.hpp"

#include "../../camera/Camera.hpp"
#include "../../devices/Display.hpp"

namespace Flounder
{
	FilterMotion::FilterMotion() :
		IPostFilter("filterMotion", "res/shaders/filters/motion.frag.spv"),
		m_lastViewMatrix(new Matrix4())
	{
	}

	FilterMotion::~FilterMotion()
	{
		delete m_lastViewMatrix;
	}

	void FilterMotion::StoreValues()
	{
#if 0
		icamera *camera = camera::get()->getCamera();
		m_shader->loadUniform4fv("projectionMatrix", *camera->getProjectionMatrix());
		m_shader->loadUniform4fv("viewMatrix", *camera->getViewMatrix());
		m_shader->loadUniform4fv("lastViewMatrix", *m_lastViewMatrix);
		m_shader->loadUniform1f("delta", static_cast<float>(Engine::Get()->getDelta()));
		m_lastViewMatrix->set(*camera->getViewMatrix());
#endif
	}
}
