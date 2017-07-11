#pragma once

#include <string>
#include "../factory/ifactoryobject.h"

namespace flounder {
	class textureobject : public ifactoryobject
	{
	private:
		std::string m_name;
	public:
		/// <summary>
		/// A new OpenGL texture object.
		/// </summary>
		textureobject();

		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~textureobject();

		/// <summary>
		/// Gets the loaded name for the texture.
		/// </summary>
		/// <returns> The textures name. </returns>
		inline std::string getName() { return m_name; }
	};
}
