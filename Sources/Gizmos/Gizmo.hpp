#pragma once

#include <optional>
#include "Engine/Engine.hpp"
#include "Maths/Transform.hpp"
#include "GizmoType.hpp"

namespace acid
{
	/// <summary>
	/// A instance of a gizmo type.
	/// </summary>
	class ACID_EXPORT Gizmo
	{
	private:
		std::shared_ptr<GizmoType> m_gizmoType;
		Transform m_transform;
		Colour m_diffuse;
	public:
		/// <summary>
		/// Creates a new gizmo object.
		/// </summary>
		/// <param name="gizmoType"> The gizmo template to build from. </param>
		/// <param name="transform"> The gizmos initial transform. </param>
		/// <param name="diffuse"> The diffuse colour for this gizmo, without a value it will be set to the types default. </param>
		Gizmo(const std::shared_ptr<GizmoType> &gizmoType, const Transform &transform, const std::optional<Colour> &diffuse = {});

		bool operator==(const Gizmo &other) const;

		bool operator!=(const Gizmo &other) const;

		std::shared_ptr<GizmoType> GetGizmoType() const { return m_gizmoType; }

		Transform GetTransform() const { return m_transform; }

		Transform &GetTransform() { return m_transform; }

		void SetTransform(const Transform &transform) { m_transform = transform; }

		Colour GetDiffuse() const { return m_diffuse; }

		void SetDiffuse(const Colour &diffuse) { m_diffuse = diffuse; }
	};
}
