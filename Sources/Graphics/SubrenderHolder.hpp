#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Subrender.hpp"

namespace acid
{
/**
 * @brief Class that contains and manages subrenders registered to a render manager.
 */
class ACID_EXPORT SubrenderHolder :
	public NonCopyable
{
public:
	/**
	 * Gets a Subrender.
	 * @tparam T The Subrender type.
	 * @return The Subrender.
	 */
	template<typename T>
	T *Get() const
	{
		for (const auto &[key, renderers] : m_stages)
		{
			for (const auto &renderer : renderers)
			{
				auto casted = dynamic_cast<T *>(renderer.get());

				if (casted != nullptr)
				{
					return casted;
				}
			}
		}

		return nullptr;
	}

	/**
	 * Adds a Subrender.
	 * @tparam T The Subrender type.
	 * @param pipelineStage The Subrender pipeline stage.
	 * @param subrender The subrender.
	 * @return The added renderer.
	 */
	template<typename T, typename... Args>
	void Add(const Pipeline::Stage &pipelineStage, std::unique_ptr<T> &&subrender)
	{
		bool emplaced = false;

		do
		{
			auto stage = m_stages.find(pipelineStage);

			if (stage == m_stages.end())
			{
				m_stages.emplace(pipelineStage, std::vector<std::unique_ptr<Subrender>>());
			}
			else
			{
				(*stage).second.emplace_back(std::move(subrender));
				emplaced = true;
			}
		} while (!emplaced);
	}

	/**
	 * Removes a Subrender.
	 * @tparam T The Subrender type.
	 */
	template<typename T>
	void Remove()
	{
		for (auto it = m_stages.begin(); it != m_stages.end(); ++it)
		{
			for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it)
			{
				auto casted = dynamic_cast<T *>((*it2).get());

				if (casted != nullptr)
				{
					(*it).second.erase(it2);

					if ((*it).second.empty())
					{
						m_stages.erase(it);
					}
				}
			}
		}
	}

	/**
	 * Clears all subrenders.
	 */
	void Clear();
private:
	friend class Graphics;

	std::map<Pipeline::Stage, std::vector<std::unique_ptr<Subrender>>> m_stages;
};
}
