#include "RendererRegister.hpp"

namespace acid
{
	RendererRegister::RendererRegister() :
		m_stages(std::map<float, std::vector<std::shared_ptr<IRenderer>>>())
	{
	}

	std::shared_ptr<IRenderer> RendererRegister::AddRenderer(const std::shared_ptr<IRenderer> &renderer)
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

		return renderer;
	}

	bool RendererRegister::RemoveRenderer(const std::shared_ptr<IRenderer> &renderer)
	{
		for (auto &[key, stage] : m_stages)
		{
			for (auto it = stage.begin(); it != stage.end(); ++it)
			{
				if (*it == renderer)
				{
					stage.erase(it);
					return true;
				}
			}
		}

		return false;
	}

	float RendererRegister::GetStageKey(const uint32_t &renderpass, const uint32_t &subpass)
	{
		return static_cast<float>(renderpass) + (static_cast<float>(subpass) / 100.0f);
	}

	float RendererRegister::GetStageKey(const GraphicsStage &graphicsStage)
	{
		return GetStageKey(graphicsStage.GetRenderpass(), graphicsStage.GetSubpass());
	}
}
