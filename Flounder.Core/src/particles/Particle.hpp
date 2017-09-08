﻿#pragma once

#include "../camera/camera.hpp"
#include "../engine/Engine.hpp"
#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"
#include "../physics/Aabb.hpp"
#include "../space/ISpatialObject.hpp"

#include "ParticleType.hpp"

namespace Flounder
{
	/// <summary>
	/// A instance of a particle type.
	/// </summary>
	class Particle :
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
		Particle(particletype *particleType, const Vector3 &position, const Vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect);

		/// <summary>
		/// Deconstructor for the particle.
		/// </summary>
		~Particle();

		/// <summary>
		/// Updates the particle.
		/// </summary>
		void update();

		bool isAlive() const { return m_transparency < 1.0f; }

		icollider *GetCollider() override { return nullptr; }

		particletype *GetParticleType() const { return m_particleType; }

		Vector3 *GetPosition() const { return m_position; }

		Vector3 *GetVelocity() const { return m_velocity; }

		Vector3 *GetChange() const { return m_change; }

		Vector2 *GetTextureOffset1() const { return m_textureOffset1; }

		Vector2 *GetTextureOffset2() const { return m_textureOffset2; }

		float GetLifeLength() const { return m_lifeLength; }

		float GetRotation() const { return m_rotation; }

		float GetScale() const { return m_scale; }

		float GetGravityEffect() const { return m_gravityEffect; }

		float GetElapsedTime() const { return m_elapsedTime; }

		float GetTransparency() const { return m_transparency; }

		float GetTextureBlendFactor() const { return m_textureBlendFactor; }

		float GetDistanceToCamera() const { return m_distanceToCamera; }

		bool operator<(const Particle &other) const;
	private:
		Vector2 *UpdateTextureOffset(Vector2 *offset, const int &index) const;
	};
}
