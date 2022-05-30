#pragma once

#include "Models/Model.hpp"

namespace acid {
/**
 * @brief Resource that represents a disk model.
 */
class ACID_EXPORT DiskModel : public Model::Registrar<DiskModel> {
	inline static const bool Registered = Register("disk");
public:
	/**
	 * Creates a new disk model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The disk model with the requested values.
	 */
	static std::shared_ptr<DiskModel> Create(const Node &node);

	/**
	 * Creates a new disk model, or finds one with the same values.
	 * @param innerRadius The inner radius.
	 * @param outerRadius The outer radius.
	 * @param slices The amount of vertices that will be used to create the circular footprint.
	 * @param loops The amount of circles that will be created between the inner and outer radius.
	 * @return The disk model with the requested values.
	 */
	static std::shared_ptr<DiskModel> Create(float innerRadius = 0.5f, float outerRadius = 1.0f, uint32_t slices = 20, uint32_t loops = 4);

	/**
	 * Creates a new disk model.
	 * @param innerRadius The inner radius.
	 * @param outerRadius The outer radius.
	 * @param slices The amount of vertices that will be used to create the circular footprint.
	 * @param loops The amount of circles that will be created between the inner and outer radius.
	 * @param load If this resource will be loaded immediately, otherwise {@link DiskModel#Load} can be called later.
	 */
	explicit DiskModel(float innerRadius = 0.5f, float outerRadius = 1.0f, uint32_t slices = 20, uint32_t loops = 4, bool load = true);

	friend const Node &operator>>(const Node &node, DiskModel &model);
	friend Node &operator<<(Node &node, const DiskModel &model);

private:
	void Load();

	float innerRadius, outerRadius;
	uint32_t slices, loops;
};
}
