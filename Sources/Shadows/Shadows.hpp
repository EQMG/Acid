#pragma once

#include "Engine/Engine.hpp"
#include "Maths/Vector3.hpp"
#include "ShadowBox.hpp"

namespace acid
{
/**
 * @brief Module used for managing a shadow map.
 */
class Shadows :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Shadows *Get() { return Engine::Get()->GetModule<Shadows>(); }

	Shadows();

	void Update() override;

	const Vector3f &GetLightDirection() const { return m_lightDirection; }

	void SetLightDirection(const Vector3f &lightDirection) { m_lightDirection = lightDirection; }

	const uint32_t &GetShadowSize() const { return m_shadowSize; }

	void SetShadowSize(const uint32_t &shadowSize) { m_shadowSize = shadowSize; }

	const int32_t &GetShadowPcf() const { return m_shadowPcf; }

	void SetShadowPcf(const int32_t &shadowPcf) { m_shadowPcf = shadowPcf; }

	const float &GetShadowBias() const { return m_shadowBias; }

	void SetShadowBias(const float &shadowBias) { m_shadowBias = shadowBias; }

	const float &GetShadowDarkness() const { return m_shadowDarkness; }

	void SetShadowDarkness(const float &shadowDarkness) { m_shadowDarkness = shadowDarkness; }

	const float &GetShadowTransition() const { return m_shadowTransition; }

	void SetShadowTransition(const float &shadowTransition) { m_shadowTransition = shadowTransition; }

	const float &GetShadowBoxOffset() const { return m_shadowBoxOffset; }

	void SetShadowBoxOffset(const float &shadowBoxOffset) { m_shadowBoxOffset = shadowBoxOffset; }

	const float &GetShadowBoxDistance() const { return m_shadowBoxDistance; }

	void SetShadowBoxDistance(const float &shadowBoxDistance) { m_shadowBoxDistance = shadowBoxDistance; }

	/**
	 * Get the shadow box, so that it can be used by other class to test if engine.entities are inside the box.
	 * @return The shadow box.
	 */
	const ShadowBox &GetShadowBox() const { return m_shadowBox; }

private:
	Vector3f m_lightDirection;

	uint32_t m_shadowSize;
	int32_t m_shadowPcf;
	float m_shadowBias;
	float m_shadowDarkness;
	float m_shadowTransition;

	float m_shadowBoxOffset;
	float m_shadowBoxDistance;

	ShadowBox m_shadowBox;
};
}
