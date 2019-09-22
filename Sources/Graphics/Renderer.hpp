#pragma once

#include "StdAfx.hpp"
#include "RenderStage.hpp"
#include "SubrenderHolder.hpp"

namespace acid {
/**
 * @brief Class used to manage {@link Subrender} objects to create a list of render pass.
 */
class ACID_EXPORT Renderer {
	friend class Graphics;
public:
	/**
	 * Creates a new renderer, fill {@link m_renderStages} in your subclass of this.
	 */
	Renderer() = default;
	
	virtual ~Renderer() = default;

	/**
	 * Run when switching to this scene from another, use this method to create {@link Subrender}'s.
	 */
	virtual void Start() = 0;

	/**
	 * Run when updating the renderer manager.
	 */
	virtual void Update() = 0;

	/**
	 * Checks whether a Subrender exists or not.
	 * @tparam T The Subrender type.
	 * @return If the Subrender has the System.
	 */
	 /*template<typename T>
	 bool HasSubrender() const  {
		 return m_subrenderHolder.Has<T>();
	 }*/

	 /**
	  * Gets a Subrender.
	  * @tparam T The Subrender type.
	  * @return The Subrender.
	  */
	template<typename T>
	T *GetSubrender() const {
		return m_subrenderHolder.Get<T>();
	}

	RenderStage *GetRenderStage(uint32_t index) const {
		if (m_renderStages.empty() || m_renderStages.size() < index) {
			return nullptr;
		}

		return m_renderStages.at(index).get();
	}

protected:
	/**
	 * Adds a Subrender.
	 * @tparam T The Subrender type.
	 * @param pipelineStage The Subrender pipeline stage.
	 * @tparam Args The constructor arg types.
	 * @param args The constructor arguments.
	 */
	template<typename T, typename... Args>
	T *AddSubrender(const Pipeline::Stage &pipelineStage, Args &&...args) {
		return m_subrenderHolder.Add<T>(pipelineStage, std::make_unique<T>(pipelineStage, std::forward<Args>(args)...));
	}

	/**
	 * Removes a Subrender.
	 * @tparam T The Subrender type.
	 */
	template<typename T>
	void RemoveSubrender() {
		m_subrenderHolder.Remove<T>();
	}

	/**
	 * Clears all Subrenders.
	 */
	void ClearSubrenders() {
		m_subrenderHolder.Clear();
	}

	void AddRenderStage(std::unique_ptr<RenderStage> &&renderStage) {
		m_renderStages.emplace_back(std::move(renderStage));
	}

private:
	bool m_started = false;
	SubrenderHolder m_subrenderHolder;
	std::vector<std::unique_ptr<RenderStage>> m_renderStages;
};
}
