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
	static std::shared_ptr<ModelCube> Create(const Vector3 &extents = Vector3::One);

	/**
	 * Creates a new cube model.
	 * @param extents The size (width, height, depth) to load from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelCube#Load} can be called later.
	 */
	explicit ModelCube(const Vector3 &extents = Vector3::One, const bool &load = true);

	void Load() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

private:
	Vector3 m_extents;
};
}
