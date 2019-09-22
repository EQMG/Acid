#pragma once

#include "Models/Model.hpp"

namespace acid {
/**
 * @brief Resource that represents a cube model.
 */
class ACID_EXPORT ModelCube : public Model::Registrar<ModelCube> {
public:
	/**
	 * Creates a new cube model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The cube model with the requested values.
	 */
	static std::shared_ptr<ModelCube> Create(const Node &node);

	/**
	 * Creates a new cube model, or finds one with the same values.
	 * @param extents The size (width, height, depth) to load from.
	 * @return The cube model with the requested values.
	 */
	static std::shared_ptr<ModelCube> Create(const Vector3f &extents = Vector3f(1.0f));

	/**
	 * Creates a new cube model.
	 * @param extents The size (width, height, depth) to load from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelCube#Load} can be called later.
	 */
	explicit ModelCube(const Vector3f &extents = Vector3f(1.0f), bool load = true);

	friend const Node &operator>>(const Node &node, ModelCube &model);
	friend Node &operator<<(Node &node, const ModelCube &model);

private:
	void Load();
	
	static bool registered;

	Vector3f m_extents;
};
}
