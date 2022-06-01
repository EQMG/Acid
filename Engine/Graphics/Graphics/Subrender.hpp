#pragma once

#include "Commands/CommandBuffer.hpp"
#include "Graphics/Pipelines/Pipeline.hpp"
#include "Utils/NonCopyable.hpp"
#include "Utils/TypeInfo.hpp"

namespace acid {
/**
 * @brief Represents a render pipeline that is used to render a type of pipeline.
 */
class ACID_GRAPHICS_EXPORT Subrender : NonCopyable {
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

template class ACID_GRAPHICS_EXPORT TypeInfo<Subrender>;
}
