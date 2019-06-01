#pragma once

#include "Models/Model.hpp"

namespace acid
{
/**
 * @brief Resource that represents a disk model.
 */
class ACID_EXPORT ModelDisk :
	public Model
{
public:
	/**
	 * Creates a new disk model, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The disk model with the requested values.
	 */
	static std::shared_ptr<ModelDisk> Create(const Metadata &metadata);

	/**
	 * Creates a new disk model, or finds one with the same values.
	 * @param innerRadius The inner radius.
	 * @param outerRadius The outer radius.
	 * @param slices The amount of vertices that will be used to create the circular footprint.
	 * @param loops The amount of circles that will be created between the inner and outer radius.
	 * @return The disk model with the requested values.
	 */
	static std::shared_ptr<ModelDisk> Create(const float &innerRadius = 0.5f, const float &outerRadius = 1.0f, const uint32_t &slices = 20, const uint32_t &loops = 4);

	/**
	 * Creates a new disk model.
	 * @param innerRadius The inner radius.
	 * @param outerRadius The outer radius.
	 * @param slices The amount of vertices that will be used to create the circular footprint.
	 * @param loops The amount of circles that will be created between the inner and outer radius.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelDisk#Load} can be called later.
	 */
	explicit ModelDisk(const float &innerRadius = 0.5f, const float &outerRadius = 1.0f, const uint32_t &slices = 20, const uint32_t &loops = 4, const bool &load = true);

	void Load() override;

	friend const Metadata &operator>>(const Metadata &metadata, ModelDisk &model);

	friend Metadata &operator<<(Metadata &metadata, const ModelDisk &model);

private:
	float m_innerRadius;
	float m_outerRadius;
	uint32_t m_slices;
	uint32_t m_loops;
};
}
