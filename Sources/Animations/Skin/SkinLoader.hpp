#pragma once

#include <vector>
#include "Serialized/Metadata.hpp"
#include "VertexWeights.hpp"

namespace acid
{
	class ACID_EXPORT SkinLoader
	{
	private:
		Metadata *m_skinData;

		uint32_t m_maxWeights;

		std::vector<std::string> m_jointOrder;
		std::vector<VertexWeights> m_vertexWeights;
	public:
		SkinLoader(Metadata *libraryControllers, const uint32_t &maxWeights);

		std::vector<std::string> GetJointOrder() const { return m_jointOrder; }

		std::vector<VertexWeights> GetVertexWeights() const { return m_vertexWeights; }
	private:
		void LoadJointsList();

		std::vector<float> LoadWeights();

		std::vector<uint32_t> GetEffectiveJointsCounts(Metadata *weightsDataNode);

		void GetSkinWeights(Metadata *weightsDataNode, const std::vector<uint32_t> &counts, const std::vector<float> &weights);
	};
}
