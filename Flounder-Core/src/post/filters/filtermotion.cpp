#include "filtermotion.h"

namespace flounder
{
	filtermotion::filtermotion() :
		ipostfilter("filterMotion", "res/shaders/filters/motionFragment.glsl")
	{
		m_lastViewMatrix = new matrix4x4();
	}

	filtermotion::~filtermotion()
	{
		delete m_lastViewMatrix;
	}

	void filtermotion::storeValues()
	{
		icamera *camera = camera::get()->getCamera();
		m_shader->loadUniform("projectionMatrix", camera->getProjectionMatrix());
		m_shader->loadUniform("viewMatrix", camera->getViewMatrix());
		m_shader->loadUniform("lastViewMatrix", m_lastViewMatrix);
		m_shader->loadUniform("delta", static_cast<float>(framework::get()->getDelta()));
		m_lastViewMatrix->set(*camera->getViewMatrix());
	}
}
