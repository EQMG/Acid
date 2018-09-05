#pragma once

#include <vector>
#include "Serialized/Node.hpp"
#include "VertexSkinData.hpp"

namespace acid
{
	class ACID_EXPORT SkinLoader
	{
	private:
		std::shared_ptr<Node> m_skinData;

		uint32_t m_maxWeights;

		std::vector<std::string> m_jointOrder;
		std::vector<VertexSkinData> m_verticesSkinData;
	public:
		SkinLoader(const std::shared_ptr<Node> &libraryControllers, const uint32_t &maxWeights);

		~SkinLoader();

		std::vector<std::string> GetJointOrder() const { return m_jointOrder; }

		std::vector<VertexSkinData> GetVerticesSkinData() const { return m_verticesSkinData; }
	private:
		void LoadJointsList();

		std::vector<float> LoadWeights();

		std::vector<uint32_t> GetEffectiveJointsCounts(std::shared_ptr<Node> &weightsDataNode);

		void GetSkinData(std::shared_ptr<Node> &weightsDataNode, const std::vector<uint32_t> &counts, const std::vector<float> &weights);
	};
}
