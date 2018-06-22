#pragma once

#include <map>
#include <memory>
#include "Pipelines/PipelineCreate.hpp"
#include "Renderpass/RenderpassCreate.hpp"

namespace fl
{
	class IRenderer;

	/// <summary>
	/// A extension used with IRenderer's to define a master renderer.
	/// </summary>
	class FL_EXPORT IManagerRender
	{
	private:
		std::map<float, std::vector<std::shared_ptr<IRenderer>>> m_stages;
	public:
		/// <summary>
		/// Creates a new master renderer.
		/// </summary>
		IManagerRender(const std::vector<RenderpassCreate *> &renderpassCreate);

		/// <summary>
		/// Deconstructor for the master renderer.
		/// </summary>
		virtual ~IManagerRender();

		/// <summary>
		/// Run when rendering the master renderer.
		/// </summary>
		virtual void Update() = 0;

		std::map<float, std::vector<std::shared_ptr<IRenderer>>> GetStages() const { return m_stages; }

		template<typename T>
		std::shared_ptr<T> GetRenderer(const bool &allowDisabled = false)
		{
			std::shared_ptr<T> alternative = nullptr;

			for (auto &stage : m_stages)
			{
				for (auto &renderer : stage.second)
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

		std::shared_ptr<IRenderer> AddRenderer(std::shared_ptr<IRenderer> renderer);

		template<typename T, typename... Args>
		std::shared_ptr<T> AddRenderer(Args &&... args)
		{
			auto created = std::make_shared<T>(std::forward<Args>(args)...);
			AddRenderer(created);
			return created;
		}

		std::shared_ptr<IRenderer> RemoveRenderer(std::shared_ptr<IRenderer> renderer);

		template<typename T>
		std::shared_ptr<T> RemoveRenderer()
		{
			for (auto &stage : m_stages)
			{
				for (auto &renderer : stage.second)
				{
					auto casted = std::dynamic_pointer_cast<T>(renderer);

					if (casted != nullptr)
					{
						RemoveRenderer(renderer);
						return renderer;
					}
				}
			}

			return nullptr;
		}

		float GetStageKey(const unsigned int &renderpass, const uint32_t &subpass);

		float GetStageKey(const GraphicsStage &graphicsStage);
	};
}
