#pragma once

#include "Commands/CommandBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Helpers/TypeInfo.hpp"

namespace acid
{
/**
 * @brief Represents a render pipeline that is used to render a type of pipeline.
 */
class ACID_EXPORT Render
{
public:
	/**
	 * Creates a new render pipeline.
	 * @param stage The stage this renderer will be used in.
	 */
	explicit Render(Pipeline::Stage stage) :
		m_stage(std::move(stage)),
		m_enabled(true)
	{
	}

	virtual ~Render() = default;

	/**
	 * Runs the render pipeline in the current renderpass.
	 * @param commandBuffer The command buffer to record render command into.
	 */
	virtual void Record(const CommandBuffer &commandBuffer) = 0;

	const Pipeline::Stage &GetStage() const { return m_stage; }

	const bool &IsEnabled() const { return m_enabled; };

	void SetEnabled(const bool &enable) { m_enabled = enable; }

private:
	Pipeline::Stage m_stage;
	bool m_enabled;
};

template class ACID_EXPORT TypeInfo<Render>;

/**
 * Gets the Type ID for the Module.
 * @tparam T The Module type.
 * @return The Type ID.
 */
template<typename T>
TypeId GetRenderTypeId() noexcept
{
	static_assert(std::is_base_of<Render, T>::value, "T must be a Render.");

	return TypeInfo<Render>::GetTypeId<T>();
}
}
