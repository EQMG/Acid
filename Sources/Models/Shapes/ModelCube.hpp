#pragma once

#include "Models/Model.hpp"

namespace acid
{
/**
 * @brief Resource that represents a cube model.
 */
class ACID_EXPORT ModelCube :
	public Model
{
public:
	/**
	 * Creates a new cube model, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The cube model with the requested values.
	 */
	static std::shared_ptr<ModelCube> Create(const Metadata &metadata);

	/**
	 * Creates a new cube model, or finds one with the same values.
	 * @param extents The size (width, height, depth) to load from.
	 * @return The cube model with the requested values.
	 */
	static std::shared_ptr<ModelCube> Create(const Vector3f &extents = {1.0f});

	/**
	 * Creates a new cube model.
	 * @param extents The size (width, height, depth) to load from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelCube#Load} can be called later.
	 */
	explicit ModelCube(const Vector3f &extents = {1.0f}, const bool &load = true);

	void Load() override;

	friend const Metadata &operator>>(const Metadata &metadata, ModelCube &model);

	friend Metadata &operator<<(Metadata &metadata, const ModelCube &model);

private:
	Vector3f m_extents;
};
}
