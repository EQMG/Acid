#pragma once

#include "Models/Model.hpp"

namespace acid
{
/**
 * A resource that represents a sphere model.
 */
class ACID_EXPORT ModelSphere :
	public Model
{
public:
	/**
	 * Creates a new sphere model, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The sphere model with the requested values.
	 */
	static std::shared_ptr<ModelSphere> Create(const Metadata &metadata);

	/**
	 * Creates a new sphere model, or finds one with the same values.
	 * @param radius The radius.
	 * @param latitudeBands The amount of vertices that will make up the spheres latitude.
	 * @param longitudeBands The amount of vertices that will make up the spheres longitude.
	 * @return The sphere model with the requested values.
	 */
	static std::shared_ptr<ModelSphere> Create(const float &radius = 1.0f, const uint32_t &latitudeBands = 20, const uint32_t &longitudeBands = 20);

	/**
	 * Creates a new sphere model.
	 * @param radius The radius.
	 * @param latitudeBands The amount of vertices that will make up the spheres latitude.
	 * @param longitudeBands The amount of vertices that will make up the spheres longitude.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelSphere#Load} can be called later.
	 */
	explicit ModelSphere(const float &radius = 1.0f, const uint32_t &latitudeBands = 20, const uint32_t &longitudeBands = 20, const bool &load = true);

	void Load() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

private:
	float m_radius;
	uint32_t m_latitudeBands;
	uint32_t m_longitudeBands;
};
}
