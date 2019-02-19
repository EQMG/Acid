#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a sphere model.
	/// </summary>
	class ACID_EXPORT ModelSphere :
		public Model
	{
	public:
		/// <summary>
		/// Will find an existing sphere model with the same values, or create a new sphere model.
		/// </summary>
		/// <param name="metadata"> The metadata to decode values from. </param>
		static std::shared_ptr<ModelSphere> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing sphere model with the same values, or create a new sphere model.
		/// </summary>
		/// <param name="radius"> The radius. </param>
		/// <param name="latitudeBands"> The amount of vertices that will make up the spheres latitude. </param>
		/// <param name="longitudeBands"> The amount of vertices that will make up the spheres longitude. </param>
		static std::shared_ptr<ModelSphere> Create(const float &radius = 1.0f, const uint32_t &latitudeBands = 20, const uint32_t &longitudeBands = 20);

		/// <summary>
		/// Creates a new sphere model.
		/// </summary>
		/// <param name="radius"> The radius. </param>
		/// <param name="latitudeBands"> The amount of vertices that will make up the spheres latitude. </param>
		/// <param name="longitudeBands"> The amount of vertices that will make up the spheres longitude. </param>
		/// <param name="load"> If this resource will load immediately, otherwise <seealso cref="#Load()"/> can be called. </param>
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
