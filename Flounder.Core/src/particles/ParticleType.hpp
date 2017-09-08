#pragma once

#include <string>

#include "../textures/Texture.hpp"

namespace Flounder
{
	/// <summary>
	/// A definition for what a particle should act and look like.
	/// </summary>
	class ParticleType
	{
	private:
		std::string m_name;
		texture *m_texture;
		float m_lifeLength;
		float m_scale;
	public:
		/// <summary>
		/// Creates a new particle type.
		/// </summary>
		/// <param name="name"> The name for the particle type. </param>
		/// <param name="texture"> The particles texture. </param>
		/// <param name="lifeLength"> The averaged life length for the particle. </param>
		/// <param name="scale"> The averaged scale for the particle. </param>
		ParticleType(const std::string &name, texture *texture, const float &lifeLength, const float &scale);

		/// <summary>
		/// Deconstructor for the particle type.
		/// </summary>
		~ParticleType();

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = move(name); }

		texture *GetTexture() const { return m_texture; }

		void SetTexture(texture *texture) { m_texture = texture; }

		float GetLifeLength() const { return m_lifeLength; }

		void SetLifeLength(const float &lifeLength) { m_lifeLength = lifeLength; }

		float GetScale() const { return m_scale; }

		void SetScale(const float &scale) { m_scale = scale; }
	};
}
