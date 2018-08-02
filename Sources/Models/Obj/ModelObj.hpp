#pragma once

#include <functional>
#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"
#include "Models/VertexModel.hpp"
#include "Models/VertexModelData.hpp"

namespace acid
{
	class ACID_EXPORT ModelObj :
		public Model
	{
	public:
		static std::shared_ptr<ModelObj> Resource(const std::string &filename)
		{
			std::string realFilename = Files::SearchFile(filename);
			auto resource = Resources::Get()->Get(realFilename);

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ModelObj>(resource);
			}

			auto result = std::make_shared<ModelObj>(realFilename);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		ModelObj(const std::string &filename);

	private:
		VertexModelData *ProcessDataVertex(const Vector3 &vertex, std::vector<VertexModelData *> *vertices, std::vector<uint32_t> *indices);

		VertexModelData *DealWithAlreadyProcessedDataVertex(VertexModelData *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<uint32_t> *indices, std::vector<VertexModelData *> *vertices);

		void CalculateTangents(VertexModelData *v0, VertexModelData *v1, VertexModelData *v2, std::vector<Vector2> *uvs);
	};
}
