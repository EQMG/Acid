#include "IManagerRender.hpp"

#include "IRenderer.hpp"

namespace acid
{
	IManagerRender::IManagerRender(const std::vector<RenderpassCreate> &renderpassCreate) :
		m_stages(std::map<float, std::vector<std::shared_ptr<IRenderer>>>())
	{
		Renderer::Get()->CreateRenderpass(renderpassCreate);
	}

	IManagerRender::~IManagerRender()
	{
	}

	std::shared_ptr<IRenderer> IManagerRender::AddRenderer(const std::shared_ptr<IRenderer> &renderer)
	{
		if (renderer == nullptr)
		{
			return nullptr;
		}

		float key = GetStageKey(renderer->GetGraphicsStage());
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

	std::shared_ptr<IRenderer> IManagerRender::RemoveRenderer(const std::shared_ptr<IRenderer> &renderer)
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

	float IManagerRender::GetStageKey(const uint32_t &renderpass, const uint32_t &subpass)
	{
		return static_cast<float>(renderpass) + (static_cast<float>(subpass) / 100.0f);
	}

	float IManagerRender::GetStageKey(const GraphicsStage &graphicsStage)
	{
		return GetStageKey(graphicsStage.GetRenderpass(), graphicsStage.GetSubpass());
	}
}
