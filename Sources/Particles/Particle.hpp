﻿#pragma once

#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "ParticleType.hpp"

namespace acid
{
	/// <summary>
	/// A instance of a particle type.
	/// </summary>
	class ACID_EXPORT Particle
	{
	public:
		/// <summary>
		/// Creates a new particle object.
		/// </summary>
		/// <param name="particleType"> The particle template to build from. </param>
		/// <param name="position"> The particles initial position. </param>
		/// <param name="velocity"> The particles initial velocity. </param>
		/// <param name="lifeLength"> The particles life length. </param>
		/// <param name="stageCycles"> The amount of times stages will be shown. </param>
		/// <param name="rotation"> The particles rotation. </param>
		/// <param name="scale"> The particles scale. </param>
		/// <param name="gravityEffect"> The particles gravity effect. </param>
		Particle(const std::shared_ptr<ParticleType> &particleType, const Vector3 &position, const Vector3 &velocity, const float &lifeLength, 
			const float &stageCycles, const float &rotation, const float &scale, const float &gravityEffect);

		/// <summary>
		/// Updates the particle.
		/// </summary>
		void Update();

		bool IsAlive() const { return m_transparency > 0.0f; }

		const std::shared_ptr<ParticleType> &GetParticleType() const { return m_particleType; }

		const Vector3 &GetPosition() const { return m_position; }

		const Vector3 &GetVelocity() const { return m_velocity; }

		const Vector3 &GetChange() const { return m_change; }

		const Vector2 &GetTextureOffset1() const { return m_textureOffset1; }

		const Vector2 &GetTextureOffset2() const { return m_textureOffset2; }

		const float &GetLifeLength() const { return m_lifeLength; }

		const float &GetRotation() const { return m_rotation; }

		const float &GetScale() const { return m_scale; }

		const float &GetGravityEffect() const { return m_gravityEffect; }

		const float &GetElapsedTime() const { return m_elapsedTime; }

		const float &GetTransparency() const { return m_transparency; }

		const float &GetTextureBlendFactor() const { return m_textureBlendFactor; }

		const float &DistanceToCamera() const { return m_distanceToCamera; }

		bool operator<(const Particle &other) const;
	private:
		Vector2 CalculateTextureOffset(const int32_t &index) const;

		std::shared_ptr<ParticleType> m_particleType;

		Vector3 m_position;
		Vector3 m_velocity;
		Vector3 m_change;

		Vector2 m_textureOffset1;
		Vector2 m_textureOffset2;

		float m_lifeLength;
		float m_stageCycles;
		float m_rotation;
		float m_scale;
		float m_gravityEffect;

		float m_elapsedTime;
		float m_transparency;
		float m_textureBlendFactor;
		float m_distanceToCamera;
	};
}
