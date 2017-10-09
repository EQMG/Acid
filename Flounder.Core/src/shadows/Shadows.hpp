#pragma once

#include "../engine/Engine.hpp"
#include "../maths/Vector3.hpp"
#include "ShadowBox.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing shadow maps in 3D worlds.
	/// </summary>
	class Shadows :
		public IModule
	{
	private:
		Vector3 *m_lightDirection;
		float m_brightnessBoost;

		int m_shadowSize;
		int m_shadowPcf;
		float m_shadowBias;
		float m_shadowDarkness;
		float m_shadowTransition;

		float m_shadowBoxOffset;
		float m_shadowBoxDistance;

		float m_shadowFactor;

		ShadowBox *m_shadowBox;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Shadows *Get()
		{
			return static_cast<Shadows*>(Engine::Get()->GetModule("shadows"));
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
	private:
		/// <summary>
		/// Creates the orthographic projection matrix.
		/// </summary>
		/// <param name="width"> Shadow box width. </param>
		/// <param name="height"> Shadow box height. </param>
		/// <param name="length"> Shadow box length. </param>
		void UpdateOrthographicProjectionMatrix(const float &width, const float &height, const float &length);

		/// <summary>
		/// Updates the "view" matrix of the light. The light itself has no position, so the "view" matrix is centered at the center of the shadow box.
		/// </summary>
		/// <param name="direction"> The light direct. </param>
		/// <param name="position"> The center of the shadow box. </param>
		void UpdateLightViewMatrix(Vector3 *direction, Vector3 *position);
	public:
		Vector3 *GetLightDirection() const { return m_lightDirection; }

		void SetLightDirection(const Vector3 &lightDirection) const { m_lightDirection->Set(lightDirection); }

		float GetBrightnessBoost() const { return m_brightnessBoost; }

		void SetBrightnessBoost(const float &brightnessBoost) { m_brightnessBoost = brightnessBoost; }

		int GetShadowSize() const { return m_shadowSize; }

		void SetShadowSize(const int &shadowSize) { m_shadowSize = shadowSize; }

		int GetShadowPcf() const { return m_shadowPcf; }

		void SetShadowPcf(const int &shadowPCF) { m_shadowPcf = shadowPCF; }

		float GetShadowBias() const { return m_shadowBias; }

		void SetShadowBias(const float &shadowBias) { m_shadowBias = shadowBias; }

		float GetShadowDarkness() const { return m_shadowDarkness; }

		void SetShadowDarkness(const float &shadowDarkness) { m_shadowDarkness = shadowDarkness; }

		float GetShadowTransition() const { return m_shadowTransition; }

		void SetShadowTransition(const float &shadowTransition) { m_shadowTransition = shadowTransition; }

		float GetShadowFactor() const { return m_shadowFactor; }

		void SetShadowFactor(const float &shadowFactor) { m_shadowFactor = shadowFactor; }

		float GetShadowBoxOffset() const { return m_shadowBoxOffset; }

		void SetShadowBoxOffset(const float &shadowBoxOffset) { m_shadowBoxOffset = shadowBoxOffset; }

		float GetShadowBoxDistance() const { return m_shadowBoxDistance; }

		void SetShadowBoxDistance(const float &shadowBoxDistance) { m_shadowBoxDistance = shadowBoxDistance; }

		/// <summary> Get the shadow box, so that it can be used by other class to test if engine.entities are inside the box. </summary>
		/// <returns> The shadow box. </returns>
		ShadowBox *GetShadowBox() const { return m_shadowBox; }
	};
}
