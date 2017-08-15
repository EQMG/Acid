#pragma once

#include "../camera/camera.h"
#include "../maths/vector3.h"
#include "../maths/matrix4x4.h"
#include "../framework/framework.h"
#include "../physics/aabb.h"

#include "shadowbox.h"

namespace flounder
{
	/// <summary>
	/// A module used for managing shadow maps in 3D worlds.
	/// </summary>
	class shadows :
		public imodule
	{
	private:
		vector3 *m_lightDirection;
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
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline shadows *get()
		{
			return static_cast<shadows*>(framework::get()->getInstance("shadows"));
		}

		/// <summary>
		/// Creates a new shadows module.
		/// </summary>
		shadows();

		/// <summary>
		/// Deconstructor for the shadows module.
		/// </summary>
		~shadows();

		void update() override;
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
		void updateLightViewMatrix(vector3 *direction, vector3 *position);
	public:
		inline vector3 *getLightDirection() const { return m_lightDirection; }

		inline void setLightDirection(const vector3 &lightDirection) const { m_lightDirection->set(lightDirection); }

		inline float getBrightnessBoost() const { return m_brightnessBoost; }

		inline void setBrightnessBoost(const float &brightnessBoost) { m_brightnessBoost = brightnessBoost; }

		inline int getShadowSize() const { return m_shadowSize; }

		inline void setShadowSize(const int &shadowSize) { m_shadowSize = shadowSize; }

		inline int getShadowPCF() const { return m_shadowPCF; }

		inline void setShadowPCF(const int &shadowPCF) { m_shadowPCF = shadowPCF; }

		inline float getShadowBias() const { return m_shadowBias; }

		inline void setShadowBias(const float &shadowBias) { m_shadowBias = shadowBias; }

		inline float getShadowDarkness() const { return m_shadowDarkness; }

		inline void setShadowDarkness(const float &shadowDarkness) { m_shadowDarkness = shadowDarkness; }

		inline float getShadowTransition() const { return m_shadowTransition; }

		inline void setShadowTransition(const float &shadowTransition) { m_shadowTransition = shadowTransition; }

		inline float getShadowFactor() const { return m_shadowFactor; }

		inline void setShadowFactor(const float &shadowFactor) { m_shadowFactor = shadowFactor; }

		inline float getShadowBoxOffset() const { return m_shadowBoxOffset; }

		inline void setShadowBoxOffset(const float &shadowBoxOffset) { m_shadowBoxOffset = shadowBoxOffset; }

		inline float getShadowBoxDistance() const { return m_shadowBoxDistance; }

		inline void setShadowBoxDistance(const float &shadowBoxDistance) { m_shadowBoxDistance = shadowBoxDistance; }

		/// <returns> The shadow box, so that it can be used by other class to test if engine.entities are inside the box. </returns>
		shadowbox *getShadowBox() const { return m_shadowBox; }
	};
}
