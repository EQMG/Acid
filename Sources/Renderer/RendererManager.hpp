#pragma once

#include "RenderHolder.hpp"
#include "RenderStage.hpp"

namespace acid
{
class Render;

/**
 * Class used to manage {@link Render} objects to create a list of render pass.
 */
class ACID_EXPORT RendererManager
{
public:
	virtual ~RendererManager() = default;

	/**
	 * Run when updating the renderer manager.
	 */
	virtual void Update() = 0;

protected:
	/**
	 * Gets the render stages used to setup the renderer.
	 * @return The render stages.
	 */
	//std::vector<std::unique_ptr<RenderStage>> &GetRenderStages() { return m_renderStages; }

	/**
	 * Gets the render holder used by when a renderer is using this manager.
	 * @return The renderer holder.
	 */
	RenderHolder &GetRenderHolder() { return m_renderHolder; }

private:
	friend class Renderer;

	//std::vector<std::unique_ptr<RenderStage>> m_renderStages;
	RenderHolder m_renderHolder;
};
}
