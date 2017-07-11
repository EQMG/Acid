#pragma once

#include <String>
#include <map>
#include "../framework/framework.h"

namespace flounder {
	/// <summary>
	/// A module used for loading texture files.
	/// </summary>
	class textures : public imodule
	{
	private:
		std::map<std::string, ifactoryobject*> *loaded;

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
	};
}
