#include "filtermotion.hpp"

namespace flounder
{
	filtermotion::filtermotion() :
		ipostfilter("filterMotion", "res/shaders/filters/motion.frag.spv"),
		m_lastViewMatrix(new matrix4x4())
	{
	}

	filtermotion::~filtermotion()
	{
		delete m_lastViewMatrix;
	}

	void filtermotion::storeValues()
	{
#if 0
		icamera *camera = camera::get()->getCamera();
		m_shader->loadUniform4fv("projectionMatrix", *camera->getProjectionMatrix());
		m_shader->loadUniform4fv("viewMatrix", *camera->getViewMatrix());
		m_shader->loadUniform4fv("lastViewMatrix", *m_lastViewMatrix);
		m_shader->loadUniform1f("delta", static_cast<float>(framework::get()->getDelta()));
		m_lastViewMatrix->set(*camera->getViewMatrix());
#endif
	}
}
