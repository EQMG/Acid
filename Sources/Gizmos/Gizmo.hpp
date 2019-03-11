#pragma once

#include "Maths/Transform.hpp"
#include "GizmoType.hpp"

namespace acid
{
	/// <summary>
	/// A instance of a gizmo type.
	/// </summary>
	class ACID_EXPORT Gizmo
	{
	public:
		/// <summary>
		/// Creates a new gizmo object.
		/// </summary>
		/// <param name="gizmoType"> The gizmo template to build from. </param>
		/// <param name="transform"> The gizmos initial transform. </param>
		/// <param name="colour"> The colour for this gizmo, without a value it will be set to the types default. </param>
		Gizmo(const std::shared_ptr<GizmoType> &gizmoType, const Transform &transform, const std::optional<Colour> &colour = {});

		bool operator==(const Gizmo &other) const;

		bool operator!=(const Gizmo &other) const;

		std::shared_ptr<GizmoType> GetGizmoType() const { return m_gizmoType; }

		const Transform &GetTransform() const { return m_transform; }

		Transform &GetTransform() { return m_transform; }

		void SetTransform(const Transform &transform) { m_transform = transform; }

		const Colour &GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }
	private:
		std::shared_ptr<GizmoType> m_gizmoType;
		Transform m_transform;
		Colour m_colour;
	};
}
