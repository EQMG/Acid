#pragma once

#include "../camera/camera.h"
#include "../framework/framework.h"
#include "../maths/vector2.h"
#include "../maths/vector3.h"
#include "../physics/aabb.h"
#include "../space/ispatialobject.h"

#include "particletype.h"

namespace flounder
{
	/// <summary>
	/// A instance of a particle type.
	/// </summary>
	class particle :
		public ispatialobject
	{
	private:
		particletype *m_particleType;

		vector3 *m_position;
		vector3 *m_velocity;
		vector3 *m_change;
		aabb *m_aabb;

		vector2 *m_textureOffset1;
		vector2 *m_textureOffset2;

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
		particle(particletype *particleType, const vector3 &position, const vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect);

		/// <summary>
		/// Deconstructor for the particle.
		/// </summary>
		~particle();

		/// <summary>
		/// Sets this particle to a new particle object.
		/// </summary>
		/// <param name="particleType"> The particle template to build from. </param>
		/// <param name="position"> The particles initial position. </param>
		/// <param name="velocity"> The particles initial velocity. </param>
		/// <param name="lifeLength"> The particles life length. </param>
		/// <param name="rotation"> The particles rotation. </param>
		/// <param name="scale"> The particles scale. </param>
		/// <param name="gravityEffect"> The particles gravity effect.
		/// </param>
		/// <returns> this. </returns>
		particle *set(particletype *particleType, const vector3 &position, const vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect);

		/// <summary>
		/// Updates the particle.
		/// </summary>
		void update();

		inline bool isAlive() { return m_transparency < 1.0f; }

		icollider *getCollider() override { return m_aabb; }

		particletype *getParticleType() { return m_particleType; }

		vector3 *getPosition() { return m_position; }

		vector3 *getVelocity() { return m_velocity; }

		vector3 *getChange() { return m_change; }

		vector2 *getTextureOffset1() { return m_textureOffset1; }

		vector2 *getTextureOffset2() { return m_textureOffset2; }

		float getLifeLength() { return m_lifeLength; }

		float getRotation() { return m_rotation; }

		float getScale() { return m_scale; }

		float getGravityEffect() { return m_gravityEffect; }

		float getElapsedTime() { return m_elapsedTime; }

		float getTransparency() { return m_transparency; }

		float getTextureBlendFactor() { return m_textureBlendFactor; }

		float getDistanceToCamera() { return m_distanceToCamera; }
	private:
		vector2 *updateTextureOffset(vector2 *offset, const int &index);
	};
}
