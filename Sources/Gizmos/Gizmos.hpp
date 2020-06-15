#pragma once

#include "Engine/Engine.hpp"
#include "Gizmo.hpp"

namespace acid {
/**
 * @brief Module used for that manages debug gizmos.
 */
class ACID_EXPORT Gizmos : public Module::Registrar<Gizmos> {
	inline static const bool Registered = Register(Stage::Normal);
public:
	using GizmosContainer = std::map<std::shared_ptr<GizmoType>, std::vector<std::unique_ptr<Gizmo>>>;

	Gizmos();

	void Update() override;

	Gizmo *AddGizmo(std::unique_ptr<Gizmo> &&gizmo);
	void RemoveGizmo(Gizmo *gizmo);

	/**
	 * Clears all gizmos from the scene.
	 */
	void Clear();

	/**
	 * Gets a list of all gizmos.
	 * @return All gizmos.
	 */
	const GizmosContainer &GetGizmos() const { return gizmos; }

private:
	GizmosContainer gizmos;
};
}
