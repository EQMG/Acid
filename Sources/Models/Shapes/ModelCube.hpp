#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a cube model.
	/// </summary>
	class ACID_EXPORT ModelCube :
		public Model
	{
	public:
		/// <summary>
		/// Will find an existing cube model with the same values, or create a new cube model.
		/// </summary>
		/// <param name="metadata"> The metadata to decode values from. </param>
		static std::shared_ptr<ModelCube> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing cube model with the same values, or create a new cube model.
		/// </summary>
		/// <param name="width"> The width. </param>
		/// <param name="height"> The height. </param>
		/// <param name="depth"> The depth. </param>
		static std::shared_ptr<ModelCube> Create(const float &width = 1.0f, const float &height = 1.0f, const float &depth = 1.0f);

		/// <summary>
		/// Creates a new cube model.
		/// </summary>
		/// <param name="width"> The width. </param>
		/// <param name="height"> The height. </param>
		/// <param name="depth"> The depth. </param>
		/// <param name="load"> If this resource will load immediately, otherwise <seealso cref="#Load()"/> can be called. </param>
		explicit ModelCube(const float &width = 1.0f, const float &height = 1.0f, const float &depth = 1.0f, const bool &load = true);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

	private:
		float m_width;
		float m_height;
		float m_depth;
	};
}
