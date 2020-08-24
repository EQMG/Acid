#pragma once

#include "Models/Model.hpp"

namespace acid {
/**
 * @brief Resource that represents a sphere model.
 */
class ACID_EXPORT SphereModel : public Model::Registrar<SphereModel> {
	static const bool Registered;
public:
	/**
	 * Creates a new sphere model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The sphere model with the requested values.
	 */
	static std::shared_ptr<SphereModel> Create(const Node &node);

	/**
	 * Creates a new sphere model, or finds one with the same values.
	 * @param radius The radius.
	 * @param latitudeBands The amount of vertices that will make up the spheres latitude.
	 * @param longitudeBands The amount of vertices that will make up the spheres longitude.
	 * @return The sphere model with the requested values.
	 */
	static std::shared_ptr<SphereModel> Create(float radius = 1.0f, uint32_t latitudeBands = 20, uint32_t longitudeBands = 20);

	/**
	 * Creates a new sphere model.
	 * @param radius The radius.
	 * @param latitudeBands The amount of vertices that will make up the spheres latitude.
	 * @param longitudeBands The amount of vertices that will make up the spheres longitude.
	 * @param load If this resource will be loaded immediately, otherwise {@link SphereModel#Load} can be called later.
	 */
	explicit SphereModel(float radius = 1.0f, uint32_t latitudeBands = 20, uint32_t longitudeBands = 20, bool load = true);

	friend const Node &operator>>(const Node &node, SphereModel &model);
	friend Node &operator<<(Node &node, const SphereModel &model);

private:
	void Load();

	float radius;
	uint32_t latitudeBands, longitudeBands;
};
}
