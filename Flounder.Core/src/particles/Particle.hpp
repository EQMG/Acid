#pragma once

#include "../camera/camera.hpp"
#include "../engine/Engine.hpp"
#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"
#include "../physics/aabb.hpp"
#include "../space/ISpatialObject.hpp"

#include "particletype.hpp"

namespace Flounder
{
	/// <summary>
	/// A instance of a particle type.
	/// </summary>
	class particle :
		public ISpatialObject
	{
	private:
		particletype *m_particleType;

		Vector3 *m_position;
		Vector3 *m_velocity;
		Vector3 *m_change;

		Vector2 *m_textureOffset1;
		Vector2 *m_textureOffset2;

		float m_lifeLength;
		float m_rotation;
		float m_scale;
		float m_gravityEffect;

		float m_elapsedTime;
		float m_transparency;
		float m_textureBlendFactor;
		float m_distanceToCamera;
	public:
		/// <summary>
		/// Creates a new particle object.
		/// </summary>
		/// <param name="particleType"> The particle template to build from. </param>
		/// <param name="position"> The particles initial position. </param>
		/// <param name="velocity"> The particles initial velocity. </param>
		/// <param name="lifeLength"> The particles life length. </param>
		/// <param name="rotation"> The particles rotation. </param>
		/// <param name="scale"> The particles scale. </param>
		/// <param name="gravityEffect"> The particles gravity effect. </param>
		particle(particletype *particleType, const Vector3 &position, const Vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect);

		/// <summary>
		/// Deconstructor for the particle.
		/// </summary>
		~particle();

		/// <summary>
		/// Updates the particle.
		/// </summary>
		void update();

		bool isAlive() { return m_transparency < 1.0f; }

		icollider *GetCollider() override { return nullptr; }

		particletype *getParticleType() { return m_particleType; }

		Vector3 *getPosition() { return m_position; }

		Vector3 *getVelocity() { return m_velocity; }

		Vector3 *getChange() { return m_change; }

		Vector2 *getTextureOffset1() { return m_textureOffset1; }

		Vector2 *getTextureOffset2() { return m_textureOffset2; }

		float getLifeLength() { return m_lifeLength; }

		float getRotation() { return m_rotation; }

		float getScale() { return m_scale; }

		float getGravityEffect() { return m_gravityEffect; }

		float getElapsedTime() { return m_elapsedTime; }

		float getTransparency() { return m_transparency; }

		float getTextureBlendFactor() { return m_textureBlendFactor; }

		float getDistanceToCamera() { return m_distanceToCamera; }

		bool operator<(const particle &other) const;
	private:
		Vector2 *updateTextureOffset(Vector2 *offset, const int &index);
	};
}
