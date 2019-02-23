#include "Gizmos.hpp"

namespace acid
{
	Gizmos::Gizmos() :
		m_mutex(std::mutex()),
		m_gizmos(std::map<std::shared_ptr<GizmoType>, std::vector<std::unique_ptr<Gizmo>>>())
	{
	}

	void Gizmos::Update()
	{
		/*std::lock_guard<std::mutex> lock(m_mutex);

		for (auto &[gizmoType, gizmos] : m_gizmos)
		{
			gizmoType->Update(gizmos);
		}*/
	}

	Gizmo *Gizmos::AddGizmo(Gizmo *gizmo)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = m_gizmos.find(gizmo->GetGizmoType());

		if (it == m_gizmos.end())
		{
			m_gizmos.emplace(gizmo->GetGizmoType(), std::vector<std::unique_ptr<Gizmo>>());
			it = m_gizmos.find(gizmo->GetGizmoType());
		}

		(*it).second.emplace_back(gizmo);
		return gizmo;
	}

	void Gizmos::RemoveGizmo(Gizmo *gizmo)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto &[gizmoType, gizmos] : m_gizmos)
		{
			gizmos.erase(std::remove_if(gizmos.begin(), gizmos.end(), [&](std::unique_ptr<Gizmo> &g)
			{
				return g.get() == gizmo;
			}), gizmos.end());
		}
	}

	void Gizmos::Clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_gizmos.clear();
	}
}
