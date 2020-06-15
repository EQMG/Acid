#pragma once

#include "Utils/NonCopyable.hpp"
#include "Pipelines/Pipeline.hpp"
#include "Subrender.hpp"

namespace acid {
/**
 * @brief Class that contains and manages subrenders registered to a render manager.
 */
class ACID_EXPORT SubrenderHolder : NonCopyable {
	friend class Graphics;
public:
	/**
	 * Gets a Subrender.
	 * @tparam T The Subrender type.
	 * @return The Subrender.
	 */
	template<typename T>
	T *Get() const {
		const auto typeId = TypeInfo<Subrender>::GetTypeId<T>();

		if (auto it = subrenders.find(typeId);  it != subrenders.end() && it->second)
			return static_cast<T *>(it->second.get());

		//throw std::runtime_error("Subrender Holder does not have requested Subrender");
		return nullptr;
	}

	/**
	 * Adds a Subrender.
	 * @tparam T The Subrender type.
	 * @param stage The Subrender pipeline stage.
	 * @param subrender The subrender.
	 * @return The added renderer.
	 */
	template<typename T, typename... Args>
	T *Add(const Pipeline::Stage &stage, std::unique_ptr<T> &&subrender) {
		// Remove previous Subrender, if it exists.
		//Remove<T>();

		const auto typeId = TypeInfo<Subrender>::GetTypeId<T>();

		// Insert the stage value
		stages.insert({StageIndex(stage, subrenders.size()), typeId});

		// Then, add the Subrender
		subrenders[typeId] = std::move(subrender);
		return static_cast<T *>(subrenders[typeId].get());
	}

	/**
	 * Removes a Subrender.
	 * @tparam T The Subrender type.
	 */
	template<typename T>
	void Remove() {
		const auto typeId = TypeInfo<Subrender>::GetTypeId<T>();

		// Remove the stage value for this Subrender.
		RemoveSubrenderStage(typeId);

		// Then, remove the Subrender.
		subrenders.erase(typeId);
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

	/// List of all Subrenders.
	std::unordered_map<TypeId, std::unique_ptr<Subrender>> subrenders;
	/// List of subrender stages.
	std::multimap<StageIndex, TypeId> stages;
};
}
