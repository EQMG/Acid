#pragma once

#include <Flounder.hpp>
#include "particles/rendererparticles.hpp"
#include "skyboxes/rendererskyboxes.hpp"

using namespace Flounder;

class ManagerRender :
	public IManagerRender
{
private:
	Vector4 m_infinity;

	RendererTest *m_rendererTest;
public:
	ManagerRender();

	~ManagerRender();

	void CreateCommands(size_t i, std::vector<VkCommandBuffer> commandBuffers) override;

	void Render() override;
};
