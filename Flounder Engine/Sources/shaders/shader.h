#pragma once

#include <vector>
#include <map>
#include <stdarg.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../helpers/helperfile.h"
#include "../helpers/helperstring.h"

namespace flounder {
	/// <summary>
	/// A struct that represents a shader type.
	/// </summary>
	class shadertype {
	public:
		int m_shaderType;
		std::string m_file;
		std::string m_string;
		GLuint m_shaderID;

		shadertype(int shaderType, std::string file)
		{
			m_shaderType = shaderType;
			m_file = file;
		}

		~shadertype()
		{
		}
	};

	/// <summary>
	/// Class that represents a loaded shader.
	/// </summary>
	class shader {
	private:
		std::vector<shadertype*> *m_shaderTypes;
		std::vector<std::string> *m_layoutLocations;
		std::vector<std::string> *m_layoutBindings;
		std::map<std::string, std::vector<std::string>> *m_uniforms;

		GLuint m_programID;
	public:
		/// <summary>
		/// Creates a new shader.
		/// </summary>
		/// <param name="n_args"> The number of shader types being loaded. </param>
		/// <param name="..."> The shader types being loaded. </param>
		shader(int n_args, ...);

		~shader();

		/// <summary>
		/// Starts the shader program.
		/// </summary>
		void start();

		/// <summary>
		/// Stops the shader program.
		/// </summary>
		void stop();
	private:
		void load();
	};
}