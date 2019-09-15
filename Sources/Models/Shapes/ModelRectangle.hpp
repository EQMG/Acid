#pragma once

#include "Models/Model.hpp"

namespace acid {
/**
 * @brief Resource that represents a rectangle model.
 */
class ACID_EXPORT ModelRectangle : public Model {
public:
	/**
	 * Creates a new rectangle model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The rectangle model with the requested values.
	 */
	static std::shared_ptr<ModelRectangle> Create(const Node &node);

	/**
	 * Creates a new rectangle model, or finds one with the same values.
	 * @param min The minimum extents.
	 * @param max The maximum extents.
	 * @return The rectangle model with the requested values.
	 */
	static std::shared_ptr<ModelRectangle> Create(float min = 0.0f, float max = 1.0f);

	/**
	 * Creates a new rectangle model.
	 * @param min The minimum extents.
	 * @param max The maximum extents.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelRectangle#Load} can be called later.
	 */
	explicit ModelRectangle(float min = 0.0f, float max = 1.0f, bool load = true);

	friend const Node &operator>>(const Node &node, ModelRectangle &model);
	friend Node &operator<<(Node &node, const ModelRectangle &model);

private:
	void Load();

	float m_min;
	float m_max;
};
}
