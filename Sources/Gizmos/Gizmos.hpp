#pragma once

#include "Engine/Engine.hpp"
#include "Gizmo.hpp"

namespace acid
{
/**
 * @brief Module used for that manages debug gizmos.
 */
class Gizmos :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Gizmos *Get() { return Engine::Get()->GetModule<Gizmos>(); }

	Gizmos();

	void Update() override;

	Gizmo *AddGizmo(Gizmo *gizmo);

	void RemoveGizmo(Gizmo *gizmo);

	/**
	 * Clears all gizmos from the scene.
	 */
	void Clear();

	/**
	 * Gets a list of all gizmos.
	 * @return All gizmods.
	 */
	const std::map<std::shared_ptr<GizmoType>, std::vector<std::unique_ptr<Gizmo>>> &GetGizmos() const { return m_gizmos; }

private:
	std::map<std::shared_ptr<GizmoType>, std::vector<std::unique_ptr<Gizmo>>> m_gizmos;
};
}
