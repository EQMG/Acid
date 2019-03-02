#pragma once

#include <map>
#include <vector>
#include <memory>
#include "Engine/Engine.hpp"
#include "Gizmo.hpp"

namespace acid
{
	/// <summary>
	/// A module used for that manages debug gizmos.
	/// </summary>
	class ACID_EXPORT Gizmos :
		public Module
	{
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Gizmos *Get() { return Engine::Get()->GetModuleManager().Get<Gizmos>(); }

		Gizmos();

		void Update() override;

		Gizmo *AddGizmo(Gizmo *gizmo);

		void RemoveGizmo(Gizmo *gizmo);

		/// <summary>
		/// Clears all gizmods from the scene.
		/// </summary>
		void Clear();

		/// <summary>
		/// Gets a list of all gizmods.
		/// </summary>
		/// <returns> All gizmods. </returns>
		const std::map<std::shared_ptr<GizmoType>, std::vector<std::unique_ptr<Gizmo>>> &GetGizmos() const { return m_gizmos; }
	private:
		std::map<std::shared_ptr<GizmoType>, std::vector<std::unique_ptr<Gizmo>>> m_gizmos;
	};
}
