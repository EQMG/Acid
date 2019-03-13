#include "RendererContainer.hpp"

namespace acid
{
RendererContainer::RendererContainer() {}

RenderPipeline* RendererContainer::Add(RenderPipeline* renderer)
{
	if(renderer == nullptr)
		{
			return nullptr;
		}

	bool emplaced = false;

	do
		{
			auto stage = m_stages.find(renderer->GetStage());

			if(stage == m_stages.end())
				{
					m_stages.emplace(renderer->GetStage(), std::vector<std::unique_ptr<RenderPipeline>>());
				}
			else
				{
					(*stage).second.emplace_back(renderer);
					emplaced = true;
				}
		}
	while(!emplaced);

	return renderer;
}

void RendererContainer::Remove(RenderPipeline* renderer)
{
	for(auto it = m_stages.begin(); it != m_stages.end(); ++it)
		{
			for(auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it)
				{
					if((*it2).get() == renderer)
						{
							(*it).second.erase(it2);

							if((*it).second.empty())
								{
									m_stages.erase(it);
								}
						}
				}
		}
}
}
