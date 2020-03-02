#pragma once

#include "Commands/CommandBuffer.hpp"
#include "Graphics/Pipelines/Pipeline.hpp"
#include "Helpers/TypeInfo.hpp"

namespace acid {
/**
 * @brief Represents a render pipeline that is used to render a type of pipeline.
 */
class ACID_EXPORT Subrender {
public:
	/**
	 * Creates a new render pipeline.
	 * @param stage The stage this renderer will be used in.
	 */
	explicit Subrender(Pipeline::Stage stage) :
		stage(std::move(stage)) {
	}

	virtual ~Subrender() = default;

	/**
	 * Runs the render pipeline in the current renderpass.
	 * @param commandBuffer The command buffer to record render command into.
	 */
	virtual void Render(const CommandBuffer &commandBuffer) = 0;

	const Pipeline::Stage &GetStage() const { return stage; }

	bool IsEnabled() const { return enabled; }
	void SetEnabled(bool enable) { this->enabled = enable; }

private:
	bool enabled = true;
	Pipeline::Stage stage;
};

template class ACID_EXPORT TypeInfo<Subrender>;

/**
 * Gets the Type ID for the Subrender.
 * @tparam T The Subrender type.
 * @return The Type ID.
 */
template<typename T>
TypeId GetSubrenderTypeId() noexcept {
	static_assert(std::is_base_of<Subrender, T>::value, "T must be a Subrender.");

	return TypeInfo<Subrender>::GetTypeId<T>();
}
}
