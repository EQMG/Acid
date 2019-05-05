#pragma once

#include "RendererContainer.hpp"

namespace acid
{
class RenderPipeline;

/**
 * Class used to manage {@link RenderPipeline} objects to create a list of render pass.
 */
class ACID_EXPORT RenderManager
{
public:
	virtual ~RenderManager() = default;

	/**
	 * Run when updating the renderer manager.
	 */
	virtual void Update() = 0;

	/**
	 * Gets the renderer container used by when a renderer is using this manager. The container can be used to add/remove render pipelines.
	 * @return The renderer register.
	 */
	RendererContainer &GetRendererContainer() { return m_rendererContainer; }

private:
	friend class Renderer;

	RendererContainer m_rendererContainer;
};
}
