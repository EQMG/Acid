#pragma once

#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "ParticleType.hpp"

namespace fl
{
	/// <summary>
	/// A instance of a particle type.
	/// </summary>
	class FL_EXPORT Particle
	{
	private:
		std::shared_ptr<ParticleType> m_particleType;

		Vector3 m_position;

		Vector3 m_velocity;
		Vector3 m_change;

		Vector2 m_textureOffset1;
		Vector2 m_textureOffset2;

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
		Particle(std::shared_ptr<ParticleType> particleType, const Vector3 &position, const Vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect);

		/// <summary>
		/// Deconstructor for the particle.
		/// </summary>
		~Particle();

		/// <summary>
		/// Updates the particle.
		/// </summary>
		void Update();

		bool IsAlive() const { return m_transparency < 1.0f; }

		std::shared_ptr<ParticleType> GetParticleType() const { return m_particleType; }

		Vector3 GetPosition() const { return m_position; }

		Vector3 GetVelocity() const { return m_velocity; }

		Vector3 GetChange() const { return m_change; }

		Vector2 GetTextureOffset1() const { return m_textureOffset1; }

		Vector2 GetTextureOffset2() const { return m_textureOffset2; }

		float GetLifeLength() const { return m_lifeLength; }

		float GetRotation() const { return m_rotation; }

		float GetScale() const { return m_scale; }

		float GetGravityEffect() const { return m_gravityEffect; }

		float GetElapsedTime() const { return m_elapsedTime; }

		float GetTransparency() const { return m_transparency; }

		float GetTextureBlendFactor() const { return m_textureBlendFactor; }

		float DistanceToCamera() const { return m_distanceToCamera; }

		bool operator<(const Particle &other) const;

	private:
		Vector2 CalculateTextureOffset(const int &index) const;
	};
}
