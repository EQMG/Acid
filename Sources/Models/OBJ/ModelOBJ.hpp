#pragma once

#include <array>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a loaded OBJ model.
	/// </summary>
	class ACID_EXPORT ModelOBJ :
		public Model
	{
	public:
		/// <summary>
		/// Will find an existing OBJ model with the same values, or create a new OBJ model.
		/// </summary>
		/// <param name="metadata"> The metadata to decode values from. </param>
		static std::shared_ptr<ModelOBJ> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing OBJ model with the same values, or create a new OBJ model.
		/// </summary>
		/// <param name="filename"> The file to load the OBJ model from. </param>
		static std::shared_ptr<ModelOBJ> Create(const std::string &filename);

		/// <summary>
		/// Creates a new OBJ model.
		/// </summary>
		/// <param name="filename"> The file to load the model from. </param>
		/// <param name="load"> If this resource will load immediately, otherwise <seealso cref="#Load()"/> can be called. </param>
		explicit ModelOBJ(std::string filename, const bool &load = true);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		std::string m_filename;
	};
}
