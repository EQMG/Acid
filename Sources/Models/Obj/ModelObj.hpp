#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"
#include "Models/VertexModel.hpp"
#include "Models/VertexModelData.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a loaded OBJ model.
	/// </summary>
	class ACID_EXPORT ModelObj :
		public Model
	{
	public:
		/// <summary>
		/// Will find an existing OBJ model with the same filename, or create a new OBJ model.
		/// </summary>
		/// <param name="filename"> The file to load the OBJ model from. </param>
		static std::shared_ptr<ModelObj> Resource(const std::string &filename);

		/// <summary>
		/// Creates a new OBJ model.
		/// </summary>
		/// <param name="filename"> The file to load the model from. </param>
		explicit ModelObj(const std::string &filename);
	private:
		VertexModelData *ProcessDataVertex(const Vector3 &vertex, std::vector<std::unique_ptr<VertexModelData>> &vertices, std::vector<uint32_t> &indices);

		VertexModelData *DealWithAlreadyProcessedDataVertex(VertexModelData *previousVertex, const int32_t &newTextureIndex, const int32_t &newNormalIndex, std::vector<std::unique_ptr<VertexModelData>> &vertices, std::vector<uint32_t> &indices);

		void CalculateTangents(VertexModelData *v0, VertexModelData *v1, VertexModelData *v2, std::vector<Vector2> &uvs);
	};
}
