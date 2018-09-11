#pragma once

#include <map>
#include <memory>
#include "IRenderer.hpp"

namespace acid
{
	/// <summary>
	/// A class that contains and manages renderers registered to a render manager.
	/// </summary>
	class ACID_EXPORT RendererRegister
	{
	private:
		std::map<float, std::vector<std::shared_ptr<IRenderer>>> m_stages;
	public:
		RendererRegister();

		RendererRegister(const RendererRegister&) = delete; // FIXME: Temp Fix.

		RendererRegister& operator=(const RendererRegister&) = delete;

		std::map<float, std::vector<std::shared_ptr<IRenderer>>> GetStages() const { return m_stages; }

		void Clear() { m_stages.clear(); }

		/// <summary>
		/// Gets a renderer instance by type from this register.
		/// </summary>
		/// <param name="T"> The renderer type to find. </param>
		/// <param name="allowDisabled"> If disabled renderers will be returned. </param>
		/// <returns> The found renderer. </returns>
		template<typename T>
		std::shared_ptr<T> GetRenderer(const bool &allowDisabled = false)
		{
			std::shared_ptr<T> alternative = nullptr;

			for (auto &[key, stage] : m_stages)
			{
				for (auto &renderer : stage)
				{
					auto casted = std::dynamic_pointer_cast<T>(renderer);

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
		std::shared_ptr<IRenderer> AddRenderer(const std::shared_ptr<IRenderer> &renderer);

		/// <summary>
		/// Creates a renderer by type to be added this register.
		/// </summary>
		/// <param name="T"> The type of renderer to add. </param>
		/// <param name="args"> The type constructor arguments. </param>
		/// <returns> The added renderer. </returns>
		template<typename T, typename... Args>
		std::shared_ptr<T> AddRenderer(Args &&... args)
		{
			auto created = std::make_shared<T>(std::forward<Args>(args)...);
			AddRenderer(created);
			return created;
		}

		/// <summary>
		/// Removes a renderer from this register.
		/// </summary>
		/// <param name="renderer"> The renderer to remove. </param>
		/// <returns> If the renderer was removed. </returns>
		bool RemoveRenderer(const std::shared_ptr<IRenderer> &renderer);

		/// <summary>
		/// Removes a renderer by type from this register.
		/// </summary>
		/// <param name="T"> The type of renderer to remove. </param>
		/// <returns> If the renderer was removed. </returns>
		template<typename T>
		bool RemoveRenderer()
		{
			for (auto &stage : m_stages)
			{
				for (auto it = stage.second.begin(); it != stage.second.end(); ++it)
				{
					auto casted = std::dynamic_pointer_cast<T>(*it);

					if (casted != nullptr)
					{
						stage.second.erase(it);
						return true;
					}
				}
			}

			return false;
		}

		float GetStageKey(const uint32_t &renderpass, const uint32_t &subpass);

		float GetStageKey(const GraphicsStage &graphicsStage);
	};
}
