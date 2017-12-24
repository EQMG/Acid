#include "FilterMotion.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
	{
	};

	FilterMotion::FilterMotion(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Motion.frag.spv", subpass, DESCRIPTORS),
		m_lastViewMatrix(new Matrix4())
	{
	}

	FilterMotion::~FilterMotion()
	{
		delete m_lastViewMatrix;
	}

	void FilterMotion::RenderFilter(const VkCommandBuffer &commandBuffer)
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
