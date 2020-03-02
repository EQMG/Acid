#include "Gizmo.hpp"

namespace acid {
Gizmo::Gizmo(const std::shared_ptr<GizmoType> &gizmoType, const Transform &transform, const std::optional<Colour> &colour) :
	gizmoType(gizmoType),
	transform(transform),
	colour(colour ? *colour : gizmoType->GetColour()) {
}

bool Gizmo::operator==(const Gizmo &other) const {
	return gizmoType == other.gizmoType && transform == other.transform && colour == other.colour;
}

bool Gizmo::operator!=(const Gizmo &other) const {
	return !operator==(other);
}
}
