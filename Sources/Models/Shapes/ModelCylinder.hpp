#pragma once

#include "Models/Model.hpp"

namespace acid {
/**
 * @brief Resource that represents a cylinder model.
 */
class ACID_EXPORT ModelCylinder : public Model::Registrar<ModelCylinder> {
public:
	/**
	 * Creates a new cylinder model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The cylinder model with the requested values.
	 */
	static std::shared_ptr<ModelCylinder> Create(const Node &node);

	/**
	 * Creates a new cylinder model, or finds one with the same values.
	 * @param radiusBase The bases radius.
	 * @param radiusTop The tops radius.
	 * @param height The distance between the base and the top.
	 * @param slices The amount of vertices that will be used to create the circular footprint.
	 * @param stacks The amount of stacks between the base and the top.
	 * @return The cylinder model with the requested values.
	 */
	static std::shared_ptr<ModelCylinder> Create(float radiusBase = 1.0f, float radiusTop = 1.0f, float height = 2.0f, uint32_t slices = 20,
		uint32_t stacks = 2);

	/**
	 * Creates a new cylinder model.
	 * @param radiusBase The bases radius.
	 * @param radiusTop The tops radius.
	 * @param height The distance between the base and the top.
	 * @param slices The amount of vertices that will be used to create the circular footprint.
	 * @param stacks The amount of stacks between the base and the top.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelCylinder#Load} can be called later.
	 */
	explicit ModelCylinder(float radiusBase = 1.0f, float radiusTop = 1.0f, float height = 2.0f, uint32_t slices = 20, uint32_t stacks = 2,
		bool load = true);

	friend const Node &operator>>(const Node &node, ModelCylinder &model);
	friend Node &operator<<(Node &node, const ModelCylinder &model);

private:
	void Load();

	static bool registered;

	float m_radiusBase;
	float m_radiusTop;
	float m_height;
	uint32_t m_slices;
	uint32_t m_stacks;
};
}
