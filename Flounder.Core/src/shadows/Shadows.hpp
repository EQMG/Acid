#pragma once

#include "../camera/camera.hpp"
#include "../maths/Vector3.hpp"
#include "../maths/Matrix4.hpp"
#include "../engine/Engine.hpp"
#include "../physics/aabb.hpp"

#include "shadowbox.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing shadow maps in 3D worlds.
	/// </summary>
	class shadows :
		public IModule
	{
	private:
		Vector3 *m_lightDirection;
		float m_brightnessBoost;

		int m_shadowSize;
		int m_shadowPCF;
		float m_shadowBias;
		float m_shadowDarkness;
		float m_shadowTransition;

		float m_shadowBoxOffset;
		float m_shadowBoxDistance;

		float m_shadowFactor;

		shadowbox *m_shadowBox;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static shadows *get()
		{
			return static_cast<shadows*>(Engine::Get()->GetModule("shadows"));
		}

		/// <summary>
		/// Creates a new shadows module.
		/// </summary>
		shadows();

		/// <summary>
		/// Deconstructor for the shadows module.
		/// </summary>
		~shadows();

		void Update() override;
	private:
		/// <summary>
		/// Creates the orthographic projection matrix.
		/// </summary>
		/// <param name="width"> Shadow box width. </param>
		/// <param name="height"> Shadow box height. </param>
		/// <param name="length"> Shadow box length. </param>
		void updateOrthographicProjectionMatrix(const float &width, const float &height, const float &length);

		/// <summary>
		/// Updates the "view" matrix of the light. The light itself has no position, so the "view" matrix is centered at the center of the shadow box.
		/// </summary>
		/// <param name="direction"> The light direct. </param>
		/// <param name="position"> The center of the shadow box. </param>
		void updateLightViewMatrix(Vector3 *direction, Vector3 *position);
	public:
		Vector3 *getLightDirection() const { return m_lightDirection; }

		void setLightDirection(const Vector3 &lightDirection) const { m_lightDirection->set(lightDirection); }

		float getBrightnessBoost() const { return m_brightnessBoost; }

		void setBrightnessBoost(const float &brightnessBoost) { m_brightnessBoost = brightnessBoost; }

		int getShadowSize() const { return m_shadowSize; }

		void setShadowSize(const int &shadowSize) { m_shadowSize = shadowSize; }

		int getShadowPCF() const { return m_shadowPCF; }

		void setShadowPCF(const int &shadowPCF) { m_shadowPCF = shadowPCF; }

		float getShadowBias() const { return m_shadowBias; }

		void setShadowBias(const float &shadowBias) { m_shadowBias = shadowBias; }

		float getShadowDarkness() const { return m_shadowDarkness; }

		void setShadowDarkness(const float &shadowDarkness) { m_shadowDarkness = shadowDarkness; }

		float getShadowTransition() const { return m_shadowTransition; }

		void setShadowTransition(const float &shadowTransition) { m_shadowTransition = shadowTransition; }

		float getShadowFactor() const { return m_shadowFactor; }

		void setShadowFactor(const float &shadowFactor) { m_shadowFactor = shadowFactor; }

		float getShadowBoxOffset() const { return m_shadowBoxOffset; }

		void setShadowBoxOffset(const float &shadowBoxOffset) { m_shadowBoxOffset = shadowBoxOffset; }

		float getShadowBoxDistance() const { return m_shadowBoxDistance; }

		void setShadowBoxDistance(const float &shadowBoxDistance) { m_shadowBoxDistance = shadowBoxDistance; }

		/// <returns> The shadow box, so that it can be used by other class to test if engine.entities are inside the box. </returns>
		shadowbox *getShadowBox() const { return m_shadowBox; }
	};
}
