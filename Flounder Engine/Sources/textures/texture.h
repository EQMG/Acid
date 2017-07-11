#pragma once

#include <string>
#include <GL/glew.h>

namespace flounder {
	class texture
	{
	public:
		/// <summary>
		/// A new OpenGL texture object.
		/// </summary>
		texture();

		texture(int n_args, ...);

		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~texture();

		int getTextureID() { return 1; }
		GLenum getGlType() { return GL_TEXTURE; }
	};
}
