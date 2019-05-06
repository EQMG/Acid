#include "ModuleHolder.hpp"

namespace acid
{
void ModuleHolder::RemoveModuleStage(const TypeId &id)
{
	for (auto it = m_stages.begin(); it != m_stages.end();)
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
}
