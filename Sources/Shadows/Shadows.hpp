#pragma once

#include "Engine/Engine.hpp"
#include "Maths/Vector3.hpp"
#include "ShadowBox.hpp"

namespace fl
{
	/// <summary>
	/// A module used for managing shadow maps in 3D worlds.
	/// </summary>
	class FL_EXPORT Shadows :
		public IModule
	{
	private:
		Vector3 m_lightDirection;

		uint32_t m_shadowSize;
		int m_shadowPcf;
		float m_shadowBias;
		float m_shadowDarkness;
		float m_shadowTransition;

		float m_shadowBoxOffset;
		float m_shadowBoxDistance;

		ShadowBox m_shadowBox;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Shadows *Get()
		{
			return Engine::Get()->GetModule<Shadows>();
		}

		/// <summary>
		/// Creates a new shadows module.
		/// </summary>
		Shadows();

		/// <summary>
		/// Deconstructor for the shadows module.
		/// </summary>
		~Shadows();

		void Update() override;

		Vector3 GetLightDirection() const { return m_lightDirection; }

		void SetLightDirection(const Vector3 &lightDirection) { m_lightDirection = lightDirection; }

		uint32_t GetShadowSize() const { return m_shadowSize; }

		void SetShadowSize(const uint32_t &shadowSize) { m_shadowSize = shadowSize; }

		int GetShadowPcf() const { return m_shadowPcf; }

		void SetShadowPcf(const int &shadowPcf) { m_shadowPcf = shadowPcf; }

		float GetShadowBias() const { return m_shadowBias; }

		void SetShadowBias(const float &shadowBias) { m_shadowBias = shadowBias; }

		float GetShadowDarkness() const { return m_shadowDarkness; }

		void SetShadowDarkness(const float &shadowDarkness) { m_shadowDarkness = shadowDarkness; }

		float GetShadowTransition() const { return m_shadowTransition; }

		void SetShadowTransition(const float &shadowTransition) { m_shadowTransition = shadowTransition; }

		float GetShadowBoxOffset() const { return m_shadowBoxOffset; }

		void SetShadowBoxOffset(const float &shadowBoxOffset) { m_shadowBoxOffset = shadowBoxOffset; }

		float GetShadowBoxDistance() const { return m_shadowBoxDistance; }

		void SetShadowBoxDistance(const float &shadowBoxDistance) { m_shadowBoxDistance = shadowBoxDistance; }

		/// <summary>
		/// Get the shadow box, so that it can be used by other class to test if engine.entities are inside the box.
		/// </summary>
		/// <returns> The shadow box. </returns>
		ShadowBox GetShadowBox() const { return m_shadowBox; }
	};
}
