#pragma once

#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a rectangle model.
	/// </summary>
	class ACID_EXPORT ModelRectangle :
		public Model
	{
	public:
		/// <summary>
		/// Will find an existing rectangle model with the same values, or create a new rectangle model.
		/// </summary>
		/// <param name="metadata"> The metadata to decode values from. </param>
		static std::shared_ptr<ModelRectangle> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing rectangle model with the same values, or create a new rectangle model.
		/// <param name="min"> The minimum value. </param>
		/// <param name="max"> The maximum value. </param>
		/// </summary>
		static std::shared_ptr<ModelRectangle> Create(const float &min = 0.0f, const float &max = 1.0f);

		/// <summary>
		/// Creates a new rectangle model.
		/// </summary>
		/// <param name="min"> The minimum value. </param>
		/// <param name="max"> The maximum value. </param>
		/// <param name="load"> If this resource will load immediately, otherwise <seealso cref="#Load()"/> can be called. </param>
		explicit ModelRectangle(const float &min = 0.0f, const float &max = 1.0f, const bool &load = true);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

	private:
		float m_min;
		float m_max;
	};
}
