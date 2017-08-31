#pragma once

#include <string>

#include "../textures/texture.hpp"

namespace Flounder
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

		std::string getName() { return m_name; }

		void setName(const std::string &name) { m_name = move(name); }

		texture *getTexture() { return m_texture; }

		void setTexture(texture *texture) { m_texture = texture; }

		float getLifeLength() { return m_lifeLength; }

		void setLifeLength(const float &lifeLength) { m_lifeLength = lifeLength; }

		float getScale() { return m_scale; }

		void setScale(const float &scale) { m_scale = scale; }
	};
}
