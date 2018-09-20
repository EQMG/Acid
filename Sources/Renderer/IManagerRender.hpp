#pragma once

#include <map>
#include <memory>
#include "Renderpass/RenderpassCreate.hpp"

namespace acid
{
	class IRenderer;

	/// <summary>
	/// A object used to manage <seealso cref="IRenderer"/>s.
	/// </summary>
	class ACID_EXPORT IManagerRender
	{
	private:
		std::vector<RenderpassCreate> m_renderpassCreates;
		bool m_started;
	public:
		/// <summary>
		/// Creates a new render manager.
		/// </summary>
		/// <param name="renderpassCreates"> The renderpass pipeline to create for this manager. </param>
		explicit IManagerRender(const std::vector<RenderpassCreate> &renderpassCreates) :
			m_renderpassCreates(renderpassCreates),
			m_started(false)
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

		std::vector<RenderpassCreate> GetRenderpassCreates() const { return m_renderpassCreates; }

		ACID_HIDDEN bool IsStarted() const { return m_started; };

		ACID_HIDDEN void SetStarted(const bool &started) { m_started = started; }
	};
}
