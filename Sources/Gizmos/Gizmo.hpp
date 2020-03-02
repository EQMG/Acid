#pragma once

#include "Maths/Transform.hpp"
#include "GizmoType.hpp"

namespace acid {
/**
 * @brief A instance of a gizmo type.
 */
class ACID_EXPORT Gizmo {
	friend class GizmoType;
public:
	/**
	 * Creates a new gizmo object.
	 * @param gizmoType The gizmo template to build from.
	 * @param transform The gizmos initial transform.
	 * @param colour The colour for this gizmo, without a value it will be set to the types default.
	 */
	Gizmo(const std::shared_ptr<GizmoType> &gizmoType, const Transform &transform, const std::optional<Colour> &colour = {});

	bool operator==(const Gizmo &other) const;
	bool operator!=(const Gizmo &other) const;

	std::shared_ptr<GizmoType> GetGizmoType() const { return gizmoType; }

	const Transform &GetTransform() const { return transform; }
	Transform &GetTransform() { return transform; }
	void SetTransform(const Transform &transform) { this->transform = transform; }

	const Colour &GetColour() const { return colour; }
	void SetColour(const Colour &colour) { this->colour = colour; }

private:
	std::shared_ptr<GizmoType> gizmoType;
	Transform transform;
	Colour colour;
};
}
