#include "IManagerRender.hpp"

#include "Renderer.hpp"

namespace fl
{
	IManagerRender::IManagerRender(const std::vector<RenderpassCreate *> &renderpassCreate) :
		m_stages(std::map<float, std::vector<std::shared_ptr<IRenderer>>>())
	{
		Renderer::Get()->CreateRenderpass(renderpassCreate);
	}

	IManagerRender::~IManagerRender()
	{
	}

	std::shared_ptr<IRenderer> IManagerRender::AddRenderer(std::shared_ptr<IRenderer> renderer)
	{
		if (renderer == nullptr)
		{
			return nullptr;
		}

		float key = StageKey(renderer->GetGraphicsStage());
		auto renderers = m_stages.find(key);

		if (renderers != m_stages.end())
		{
			(*renderers).second.emplace_back(renderer);
		}
		else
		{
			m_stages.emplace(key, std::vector<std::shared_ptr<IRenderer>>{renderer});
		}

	//	renderer->SetManager(this);
		return renderer;
	}

	std::shared_ptr<IRenderer> IManagerRender::RemoveRenderer(std::shared_ptr<IRenderer> renderer)
	{
		for (auto &stage : m_stages)
		{
			for (auto it = stage.second.begin(); it != stage.second.end(); ++it)
			{
				if (*it == renderer)
				{
				//	if (*it != nullptr)
				//	{
				//		(*it)->SetManager(nullptr);
				//	}

					stage.second.erase(it);
					return *it;
				}
			}
		}

		return nullptr;
	}

	void IManagerRender::RenderPass(const uint32_t &pass, const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Starts Rendering.
		auto startResult = Renderer::Get()->StartRenderpass(commandBuffer, pass);

		if (!startResult)
		{
			return;
		}

		// Renders subpasses.
		uint32_t subpassCount = Renderer::Get()->GetRenderStage(pass)->SubpassCount();

		for (uint32_t i = 0; i < subpassCount; i++)
		{
			float key = StageKey(pass, i);
			auto renderers = m_stages.find(key);

			if (renderers != m_stages.end())
			{
				for (auto &renderer : (*renderers).second)
				{
					if (!renderer->IsEnabled())
					{
						continue;
					}

					renderer->Render(commandBuffer, clipPlane, camera);
				}
			}

			if (i != subpassCount - 1)
			{
				Renderer::Get()->NextSubpass(commandBuffer);
			}
		}

		// Ends Rendering.
		Renderer::Get()->EndRenderpass(commandBuffer, pass);
	}

	float IManagerRender::StageKey(const unsigned int &renderpass, const uint32_t &subpass)
	{
		return static_cast<float>(renderpass) + (static_cast<float>(subpass) / 100.0f);
	}

	float IManagerRender::StageKey(const GraphicsStage &graphicsStage)
	{
		return StageKey(graphicsStage.GetRenderpass(), graphicsStage.GetSubpass());
	}
}
