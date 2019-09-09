#pragma once

#include "Models/Model.hpp"

namespace acid
{
/**
 * @brief Resource that represents a sphere model.
 */
class ACID_EXPORT ModelSphere :
	public Model
{
public:
	/**
	 * Creates a new sphere model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The sphere model with the requested values.
	 */
	static std::shared_ptr<ModelSphere> Create(const Node &node);

	/**
	 * Creates a new sphere model, or finds one with the same values.
	 * @param radius The radius.
	 * @param latitudeBands The amount of vertices that will make up the spheres latitude.
	 * @param longitudeBands The amount of vertices that will make up the spheres longitude.
	 * @return The sphere model with the requested values.
	 */
	static std::shared_ptr<ModelSphere> Create(float radius = 1.0f, uint32_t latitudeBands = 20, uint32_t longitudeBands = 20);

	/**
	 * Creates a new sphere model.
	 * @param radius The radius.
	 * @param latitudeBands The amount of vertices that will make up the spheres latitude.
	 * @param longitudeBands The amount of vertices that will make up the spheres longitude.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelSphere#Load} can be called later.
	 */
	explicit ModelSphere(float radius = 1.0f, uint32_t latitudeBands = 20, uint32_t longitudeBands = 20, const bool &load = true);

	friend const Node &operator>>(const Node &node, ModelSphere &model);

	friend Node &operator<<(Node &node, const ModelSphere &model);

private:
	void Load();

	float m_radius;
	uint32_t m_latitudeBands;
	uint32_t m_longitudeBands;
};
}
