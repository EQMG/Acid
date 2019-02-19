#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a cylinder model.
	/// </summary>
	class ACID_EXPORT ModelCylinder :
		public Model
	{
	public:
		/// <summary>
		/// Will find an existing cylinder model with the same values, or create a new cylinder model.
		/// </summary>
		/// <param name="metadata"> The metadata to decode values from. </param>
		static std::shared_ptr<ModelCylinder> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing cylinder model with the same values, or create a new cylinder model.
		/// </summary>
		/// <param name="radiusBase"> The bases radius. </param>
		/// <param name="radiusTop"> The tops radius. </param>
		/// <param name="height"> The distance between the base and the top. </param>
		/// <param name="slices"> The amount of vertices that will be used to create the circular footprint. </param>
		/// <param name="stacks"> The amount of stacks between the base and the top. </param>
		static std::shared_ptr<ModelCylinder> Create(const float &radiusBase = 1.0f, const float &radiusTop = 1.0f, const float &height = 2.0f, const uint32_t &slices = 20, const uint32_t &stacks = 2);

		/// <summary>
		/// Creates a new cylinder model.
		/// </summary>
		/// <param name="radiusBase"> The bases radius. </param>
		/// <param name="radiusTop"> The tops radius. </param>
		/// <param name="height"> The distance between the base and the top. </param>
		/// <param name="slices"> The amount of vertices that will be used to create the circular footprint. </param>
		/// <param name="stacks"> The amount of stacks between the base and the top. </param>
		/// <param name="load"> If this resource will load immediately, otherwise <seealso cref="#Load()"/> can be called. </param>
		explicit ModelCylinder(const float &radiusBase = 1.0f, const float &radiusTop = 1.0f, const float &height = 2.0f, const uint32_t &slices = 20, const uint32_t &stacks = 2, const bool &load = true);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

	private:
		float m_radiusBase;
		float m_radiusTop;
		float m_height;
		uint32_t m_slices;
		uint32_t m_stacks;
	};
}
