#include "Gizmos.hpp"

namespace acid
{
Gizmos::Gizmos() {}

void Gizmos::Update()
{
	for(auto it = m_gizmos.begin(); it != m_gizmos.end();)
		{
			if(it->second.empty())
				{
					it = m_gizmos.erase(it);
					continue;
				}

			(*it).first->Update((*it).second);
			++it;
		}
}

Gizmo* Gizmos::AddGizmo(Gizmo* gizmo)
{
	auto it = m_gizmos.find(gizmo->GetGizmoType());

	if(it == m_gizmos.end())
		{
			m_gizmos.emplace(gizmo->GetGizmoType(), std::vector<std::unique_ptr<Gizmo>>());
			it = m_gizmos.find(gizmo->GetGizmoType());
		}

	(*it).second.emplace_back(gizmo);
	return gizmo;
}

void Gizmos::RemoveGizmo(Gizmo* gizmo)
{
	auto it = m_gizmos.find(gizmo->GetGizmoType());

	if(it != m_gizmos.end())
		{
			it->second.erase(std::remove_if(it->second.begin(), it->second.end(), [&](std::unique_ptr<Gizmo>& g) { return g.get() == gizmo; }), it->second.end());
		}
}

void Gizmos::Clear()
{
	m_gizmos.clear();
}
}
