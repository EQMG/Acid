#include "Gizmos.hpp"

namespace acid
{
	Gizmos::Gizmos() :
		m_gizmos(std::map<std::shared_ptr<GizmoType>, std::vector<std::unique_ptr<Gizmo>>>())
	{
	}

	void Gizmos::Update()
	{
		for (auto &gizmoType : m_gizmos)
		{
			gizmoType.first->Update(gizmoType.second);
		}
	}

	Gizmo *Gizmos::AddGizmo(Gizmo *gizmo)
	{
		auto it = m_gizmos.find(gizmo->GetGizmoType());

		if (it == m_gizmos.end())
		{
			m_gizmos.emplace(gizmo->GetGizmoType(), std::vector<std::unique_ptr<Gizmo>>());
			it = m_gizmos.find(gizmo->GetGizmoType());
		}

		(*it).second.emplace_back(gizmo);
		return gizmo;
	}

	bool Gizmos::RemoveGizmo(Gizmo *gizmo)
	{
		for (auto it = m_gizmos.begin(); it != m_gizmos.end(); ++it)
		{
			for (auto it1 = (*it).second.begin(); it1 != (*it).second.end();)
			{
				if ((*it1).get() == gizmo)
				{
					it1 = (*it).second.erase(it1);
					return true;
				}
			}
		}

		return false;
	}

	void Gizmos::Clear()
	{
		m_gizmos.clear();
	}
}
