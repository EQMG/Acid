#include "Gizmo.hpp"

namespace acid {
Gizmo::Gizmo(const std::shared_ptr<GizmoType> &gizmoType, const Transform &transform, const std::optional<Colour> &colour) :
	gizmoType(gizmoType),
	transform(transform),
	colour(colour ? *colour : gizmoType->GetColour()) {
}

bool Gizmo::operator==(const Gizmo &rhs) const {
	return gizmoType == rhs.gizmoType && transform == rhs.transform && colour == rhs.colour;
}

bool Gizmo::operator!=(const Gizmo &rhs) const {
	return !operator==(rhs);
}
}
