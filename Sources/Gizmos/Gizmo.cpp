#include "Gizmo.hpp"

namespace acid
{
Gizmo::Gizmo(const std::shared_ptr<GizmoType>& gizmoType, const Transform& transform, const std::optional<Colour>& colour) : m_gizmoType(gizmoType), m_transform(transform), m_colour(colour ? *colour : gizmoType->GetColour()) {}

bool Gizmo::operator==(const Gizmo& other) const
{
	return m_gizmoType == other.m_gizmoType && m_transform == other.m_transform && m_colour == other.m_colour;
}

bool Gizmo::operator!=(const Gizmo& other) const
{
	return !(*this == other);
}
}
