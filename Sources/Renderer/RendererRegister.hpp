#pragma once

#include <map>
#include <memory>
#include <vector>
#include "IRenderer.hpp"

namespace acid
{
	/// <summary>
	/// A class that contains and manages renderers registered to a render manager.
	/// </summary>
	class ACID_EXPORT RendererRegister
	{
	private:
		std::map<GraphicsStage, std::vector<std::unique_ptr<IRenderer>>> m_stages;
	public:
		RendererRegister();

		RendererRegister(const RendererRegister&) = delete; 

		RendererRegister& operator=(const RendererRegister&) = delete;

		const std::map<GraphicsStage, std::vector<std::unique_ptr<IRenderer>>> &GetStages() const { return m_stages; }

		void Clear() { m_stages.clear(); }

		/// <summary>
		/// Gets a renderer instance by type from this register.
		/// </summary>
		/// <param name="T"> The renderer type to find. </param>
		/// <param name="allowDisabled"> If disabled renderers will be returned. </param>
		/// <returns> The found renderer. </returns>
		template<typename T>
		T *GetRenderer(const bool &allowDisabled = false)
		{
			T *alternative = nullptr;

			for (auto &[key, renderers] : m_stages)
			{
				for (auto &renderer : renderers)
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

		/// <summary>
		/// Adds a renderer to this register.
		/// </summary>
		/// <param name="renderer"> The renderer to add. </param>
		/// <returns> The added renderer. </returns>
		IRenderer *AddRenderer(IRenderer *renderer);

		/// <summary>
		/// Creates a renderer by type to be added this register.
		/// </summary>
		/// <param name="T"> The type of renderer to add. </param>
		/// <param name="args"> The type constructor arguments. </param>
		/// <returns> The added renderer. </returns>
		template<typename T, typename... Args>
		T *AddRenderer(Args &&... args)
		{
			auto created = new T(std::forward<Args>(args)...);
			AddRenderer(created);
			return created;
		}

		/// <summary>
		/// Removes a renderer from this register.
		/// </summary>
		/// <param name="renderer"> The renderer to remove. </param>
		/// <returns> If the renderer was removed. </returns>
		bool RemoveRenderer(IRenderer *renderer);

		/// <summary>
		/// Removes a renderer by type from this register.
		/// </summary>
		/// <param name="T"> The type of renderer to remove. </param>
		/// <returns> If the renderer was removed. </returns>
		template<typename T>
		bool RemoveRenderer()
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

						return true;
					}
				}
			}

			return false;
		}
	};
}
