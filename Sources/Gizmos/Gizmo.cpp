#include "Gizmo.hpp"

namespace acid
{
	Gizmo::Gizmo(const std::shared_ptr<GizmoType> &gizmoType, const Transform &transform, const std::optional<Colour> &diffuse) :
		m_gizmoType(gizmoType),
		m_transform(transform),
		m_diffuse(diffuse ? *diffuse : gizmoType->GetDiffuse())
	{
	}

	bool Gizmo::operator==(const Gizmo &other) const
	{
		return m_gizmoType == other.m_gizmoType && m_transform == other.m_transform && m_diffuse == other.m_diffuse;
	}

	bool Gizmo::operator!=(const Gizmo &other) const
	{
		return !(*this == other);
	}
}
