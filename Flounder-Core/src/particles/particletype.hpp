#pragma once

#include <string>

#include "../textures/texture.hpp"

namespace flounder
{
	/// <summary>
	/// A definition for what a particle should act and look like.
	/// </summary>
	class particletype
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
		particletype(const std::string &name, texture *texture, const float &lifeLength, const float &scale);

		/// <summary>
		/// Deconstructor for the particle type.
		/// </summary>
		~particletype();

		inline std::string getName() { return m_name; }

		inline void setName(const std::string &name) { m_name = std::move(name); }

		inline texture *getTexture() { return m_texture; }

		inline void setTexture(texture *texture) { m_texture = texture; }

		inline float getLifeLength() { return m_lifeLength; }

		inline void setLifeLength(const float &lifeLength) { m_lifeLength = lifeLength; }

		inline float getScale() { return m_scale; }

		inline void setScale(const float &scale) { m_scale = scale; }
	};
}
