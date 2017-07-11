#pragma once

#include <GL/glew.h>
#include "../framework/framework.h"

namespace flounder {
	/// <summary>
	/// A module used for loading texture files.
	/// </summary>
	class textures : public imodule
	{
	private:
		float m_anisotropyLevel;
	public:
		static textures *get()
		{
			return (textures*)framework::get()->getInstance("textures");
		}

		/// <summary>
		/// Creates a new texture loader class.
		/// </summary>
		textures();

		/// <summary>
		/// Deconstructor for textures.
		/// </summary>
		~textures();

		void init();

		void update();

		/// <summary>
		/// Gets the current anisotropy level for textures with anisotropy enabled to use.
		/// </summary>
		/// <returns> The current anisotropy level. </returns>
		virtual float getAnisotropyLevel() { return m_anisotropyLevel; }

		virtual void setAnisotropyLevel(const float anisotropyLevel) { m_anisotropyLevel = anisotropyLevel; }
	};
}
