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
	 * Gets a renderer instance by type from this register.
	 * @tparam T The renderer type to find.
	 * @param allowDisabled If disabled renderers will be returned.
	 * @return The found renderer.
	 */
	template<typename T>
	T *Get(const bool &allowDisabled = false) const
	{
		T *alternative = nullptr;

		for (const auto &[key, renderers] : m_stages)
		{
			for (const auto &renderer : renderers)
			{
				auto casted = dynamic_cast<T *>(renderer.get());

				if (casted != nullptr)
				{
					if (!allowDisabled && !casted->IsEnabled())
					{
						alternative = casted;
						continue;
					}

					return casted;
				}
			}
		}

		return alternative;
	}

	/**
	 * Adds a renderer to this register.
	 * @param renderer The renderer to add.
	 * @return The added renderer.
	 */
	Subrender *Add(Subrender *renderer);

	/**
	 * Creates a renderer by type to be added this register.
	 * @tparam T The type of renderer to add.
	 * @tparam Args The argument types.
	 * @param args The type constructor arguments.
	 * @return The added renderer.
	 */
	template<typename T, typename... Args>
	T *Add(Args &&... args)
	{
		auto created = new T(std::forward<Args>(args)...);
		Add(created);
		return created;
	}

	/**
	 * Removes a renderer from this register.
	 * @param renderer The renderer to remove.
	 */
	void Remove(Subrender *renderer);

	/**
	 * Removes a renderer by type from this register.
	 * @tparam T The type of renderer to remove.
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

private:
	friend class Graphics;

	std::map<Pipeline::Stage, std::vector<std::unique_ptr<Subrender>>> m_stages;
};
}
