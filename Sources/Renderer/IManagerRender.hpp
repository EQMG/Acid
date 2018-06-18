#pragma once

#include <map>
#include <memory>
#include "Renderpass/RenderpassCreate.hpp"
#include "IRenderer.hpp"

namespace fl
{
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
		virtual void Render() = 0;

		std::map<float, std::vector<std::shared_ptr<IRenderer>>> GetStages() const { return m_stages; }

		template<typename T>
		std::shared_ptr<T> GetRenderer()
		{
			for (auto &stage : m_stages)
			{
				for (auto &renderer : stage.second)
				{
					auto casted = std::dynamic_pointer_cast<T>(renderer);

					if (casted != nullptr)
					{
						return casted;
					}
				}
			}

			return nullptr;
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

	protected:
		void RenderPass(const uint32_t &pass, const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera);

	private:
		float StageKey(const unsigned int &renderpass, const uint32_t &subpass);

		float StageKey(const GraphicsStage &graphicsStage);
	};
}
