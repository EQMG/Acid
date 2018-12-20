#pragma once

#include <map>
#include <memory>
#include "Renderpass/RenderpassCreate.hpp"

namespace acid
{
	class IRenderer;

	/// <summary>
	/// A object used to manage <seealso cref="IRenderer"/> objects to create a list of render pass.
	/// </summary>
	class ACID_EXPORT RenderManager
	{
	private:
		friend class Renderer;

		bool m_started;
	protected:
		std::vector<RenderpassCreate> m_renderpassCreates;
	public:
		/// <summary>
		/// Creates a new render manager, once created fill <seealso cref="#m_renderpassCreates"/> with renderpasses.
		/// </summary>
		explicit RenderManager() :
			m_started(false),
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
	};
}
