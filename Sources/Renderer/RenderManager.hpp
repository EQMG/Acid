#pragma once

#include <map>
#include <memory>
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
	private:
		friend class Renderer;
		bool m_started;
	protected:
		RendererContainer m_rendererContainer;
		std::vector<RenderpassCreate> m_renderpassCreates;
	public:
		/// <summary>
		/// Creates a new render manager, once created fill <seealso cref="#m_renderpassCreates"/> with renderpasses.
		/// </summary>
		explicit RenderManager() :
			m_started(false),
			m_rendererContainer(RendererContainer()),
			m_renderpassCreates(std::vector<RenderpassCreate>())
		{
		}

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
	};
}
