#pragma once

#include "Renderpass/RenderpassCreate.hpp"
#include "RendererContainer.hpp"

namespace acid
{
class RenderPipeline;

/// <summary>
/// A object used to manage <seealso cref="RenderPipeline"/> objects to create a list of render pass.
/// </summary>
class ACID_EXPORT RenderManager
{
public:
	/// <summary>
	/// Creates a new render manager.
	/// </summary>
	explicit RenderManager() :
		m_started(false)
	{
	}

	virtual ~RenderManager() = default;

	/// <summary>
	/// Run when starting the renderer manager.
	/// </summary>
	virtual void Start() = 0;

	/// <summary>
	/// Run when updating the renderer manager.
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// Gets the renderer container used by when a renderer is using this manager. The container can be used to add/remove render pipelines.
	/// </summary>
	/// <returns> The renderer register. </returns>
	RendererContainer &GetRendererContainer() { return m_rendererContainer; }

private:
	friend class Renderer;

	bool m_started;
	RendererContainer m_rendererContainer;
};
}
