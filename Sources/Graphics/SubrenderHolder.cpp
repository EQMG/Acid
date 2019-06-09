#include "SubrenderHolder.hpp"

namespace acid
{
void SubrenderHolder::Clear()
{
	m_stages.clear();
}

void SubrenderHolder::RemoveSubrenderStage(const TypeId &id)
{
	for (auto it{m_stages.begin()}; it != m_stages.end();)
	{
		if (it->second == id)
		{
			it = m_stages.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SubrenderHolder::RenderStage(const Pipeline::Stage &stage, const CommandBuffer &commandBuffer)
{
	for (const auto &typeId : m_stages)
	{
		if (typeId.first.first != stage)
		{
			continue;
		}

		auto &subrender{m_subrenders[typeId.second]};

		if (subrender != nullptr)
		{
			if (subrender->IsEnabled())
			{
				subrender->Render(commandBuffer);
			}
		}
	}
}
}
