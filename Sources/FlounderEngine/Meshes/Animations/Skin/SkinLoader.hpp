#pragma once

#include <vector>
#include "../../../Files/LoadedValue.hpp"

namespace Flounder
{
	class F_EXPORT VertexSkinData
	{
	private:
		std::vector<int> m_jointIds;
		std::vector<float> m_weights;
	public:
		VertexSkinData() :
			m_jointIds(std::vector<int>()),
			m_weights(std::vector<float>())
		{
		}

		~VertexSkinData()
		{
		}

		std::vector<int> GetJointIds() const { return m_jointIds; }

		std::vector<float> GetWeights() const { return m_weights; }
	};

	class F_EXPORT SkinningData
	{
	private:
		std::vector<std::string> m_jointOrder;
		std::vector<VertexSkinData*> m_verticesSkinData;
	public:
		SkinningData(const std::vector<std::string> &jointOrder, const std::vector<VertexSkinData*> &verticesSkinData) :
			m_jointOrder(std::vector<std::string>(jointOrder)),
			m_verticesSkinData(std::vector<VertexSkinData*>(verticesSkinData))
		{
		}

		~SkinningData()
		{
		}

		std::vector<std::string> GetJointOrder() const { return m_jointOrder; }

		std::vector<VertexSkinData*> GetVerticesSkinData() const { return m_verticesSkinData; }
	};

	class F_EXPORT SkinLoader
	{
	private:
		LoadedValue *m_libraryControllers;

		int m_maxWeights;

		SkinningData *m_skinningData;
	public:
		SkinLoader(LoadedValue *libraryControllers, const int &maxWeights);

		~SkinLoader();

		SkinningData *GetData() const { return m_skinningData; }
	};
}
