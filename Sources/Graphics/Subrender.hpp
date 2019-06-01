#pragma once

#include "Commands/CommandBuffer.hpp"
#include "Graphics/Pipelines/Pipeline.hpp"
#include "Helpers/TypeInfo.hpp"

namespace acid
{
/**
 * @brief Represents a render pipeline that is used to render a type of pipeline.
 */
class ACID_EXPORT Subrender
{
public:
	/**
	 * Creates a new render pipeline.
	 * @param stage The stage this renderer will be used in.
	 */
	explicit Subrender(Pipeline::Stage stage) :
		m_stage(std::move(stage)),
		m_enabled(true)
	{
	}

	virtual ~Subrender() = default;

	/**
	 * Runs the render pipeline in the current renderpass.
	 * @param commandBuffer The command buffer to record render command into.
	 */
	virtual void Render(const CommandBuffer &commandBuffer) = 0;

	const Pipeline::Stage &GetStage() const { return m_stage; }

	const bool &IsEnabled() const { return m_enabled; };

	void SetEnabled(const bool &enable) { m_enabled = enable; }

private:
	Pipeline::Stage m_stage;
	bool m_enabled;
};

template class ACID_EXPORT TypeInfo<Subrender>;

/**
 * Gets the Type ID for the Subrender.
 * @tparam T The Subrender type.
 * @return The Type ID.
 */
template<typename T>
TypeId GetSubrenderTypeId() noexcept
{
	static_assert(std::is_base_of<Subrender, T>::value, "T must be a Subrender.");

	return TypeInfo<Subrender>::GetTypeId<T>();
}
}
