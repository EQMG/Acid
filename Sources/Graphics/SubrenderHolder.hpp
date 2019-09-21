#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Pipelines/Pipeline.hpp"
#include "Subrender.hpp"

namespace acid {
/**
 * @brief Class that contains and manages subrenders registered to a render manager.
 */
class ACID_EXPORT SubrenderHolder : public NonCopyable {
	friend class Graphics;
public:
	/**
	 * Gets a Subrender.
	 * @tparam T The Subrender type.
	 * @return The Subrender.
	 */
	template<typename T>
	T *Get() const {
		const auto typeId = GetSubrenderTypeId<T>();

		auto it = m_subrenders.find(typeId);

		if (it == m_subrenders.end() || !it->second) {
			//throw std::runtime_error("Subrender Holder does not have requested Subrender");
			return nullptr;
		}

		return static_cast<T *>(it->second.get());
	}

	/**
	 * Adds a Subrender.
	 * @tparam T The Subrender type.
	 * @param stage The Subrender pipeline stage.
	 * @param subrender The subrender.
	 * @return The added renderer.
	 */
	template<typename T, typename... Args>
	void Add(const Pipeline::Stage &stage, std::unique_ptr<T> &&subrender) {
		// Remove previous Subrender, if it exists.
		//Remove<T>();

		const auto typeId = GetSubrenderTypeId<T>();

		// Insert the stage value
		m_stages.insert({StageIndex(stage, m_subrenders.size()), typeId});

		// Then, add the Subrender
		m_subrenders[typeId] = std::move(subrender);
	}

	/**
	 * Removes a Subrender.
	 * @tparam T The Subrender type.
	 */
	template<typename T>
	void Remove() {
		const auto typeId = GetSubrenderTypeId<T>();

		// Remove the stage value for this Subrender.
		RemoveSubrenderStage(typeId);

		// Then, remove the Subrender.
		m_subrenders.erase(typeId);
	}

	/**
	 * Clears all subrenders.
	 */
	void Clear();
private:
	using StageIndex = std::pair<Pipeline::Stage, std::size_t>;

	void RemoveSubrenderStage(const TypeId &id);

	/**
	 * Iterates through all Subrenders.
	 * @param stage The Subrender stage.
	 * @param commandBuffer The command buffer to record render command into.
	 */
	void RenderStage(const Pipeline::Stage &stage, const CommandBuffer &commandBuffer);

	// List of all Subrenders.
	std::unordered_map<TypeId, std::unique_ptr<Subrender>> m_subrenders;

	// List of subrender stages.
	std::multimap<StageIndex, TypeId> m_stages;
};
}
