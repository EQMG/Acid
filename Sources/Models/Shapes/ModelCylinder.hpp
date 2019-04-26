#pragma once

#include "Models/Model.hpp"

namespace acid
{
/**
 * @brief Resource that represents a cylinder model.
 */
class ACID_EXPORT ModelCylinder :
	public Model
{
public:
	/**
	 * Creates a new cylinder model, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The cylinder model with the requested values.
	 */
	static std::shared_ptr<ModelCylinder> Create(const Metadata &metadata);

	/**
	 * Creates a new cylinder model, or finds one with the same values.
	 * @param radiusBase The bases radius.
	 * @param radiusTop The tops radius.
	 * @param height The distance between the base and the top.
	 * @param slices The amount of vertices that will be used to create the circular footprint.
	 * @param stacks The amount of stacks between the base and the top.
	 * @return The cylinder model with the requested values.
	 */
	static std::shared_ptr<ModelCylinder> Create(const float &radiusBase = 1.0f, const float &radiusTop = 1.0f, const float &height = 2.0f, const uint32_t &slices = 20,
		const uint32_t &stacks = 2);

	/**
	 * Creates a new cylinder model.
	 * @param radiusBase The bases radius.
	 * @param radiusTop The tops radius.
	 * @param height The distance between the base and the top.
	 * @param slices The amount of vertices that will be used to create the circular footprint.
	 * @param stacks The amount of stacks between the base and the top.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelCylinder#Load} can be called later.
	 */
	explicit ModelCylinder(const float &radiusBase = 1.0f, const float &radiusTop = 1.0f, const float &height = 2.0f, const uint32_t &slices = 20, const uint32_t &stacks = 2,
		const bool &load = true);

	void Load() override;

	ACID_EXPORT friend const Metadata& operator>>(const Metadata& metadata, ModelCylinder& model);

	ACID_EXPORT friend Metadata& operator<<(Metadata& metadata, const ModelCylinder& model);

private:
	float m_radiusBase;
	float m_radiusTop;
	float m_height;
	uint32_t m_slices;
	uint32_t m_stacks;
};
}
