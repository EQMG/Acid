#pragma once

#include <vector>
#include <map>
#include <stdarg.h>

#include "../devices/display.h"
#include "../helpers/helperfile.h"
#include "../helpers/helperstring.h"

#include "../maths/colour.h"
#include "../maths/matrix2x2.h"
#include "../maths/matrix3x3.h"
#include "../maths/matrix4x4.h"
#include "../maths/vector2.h"
#include "../maths/vector3.h"
#include "../maths/vector4.h"

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
		std::vector<std::string> *m_uniforms;

		GLuint m_programID;
	public:
		/// <summary>
		/// Creates a new shader.
		/// </summary>
		/// <param name="n_args"> The number of shader types being loaded. </param>
		/// <param name="..."> The shader types being loaded. </param>
		shader(int n_args, ...);

		~shader();
	private:
		void loadType(shadertype* type);

		std::string processLine(const std::string &line);

		void loadLocations();

		void deleteTypes();

		void loadBindings();

		void loadUniforms();
	public:
		GLint getUniform(std::string name);

		void loadUniform(std::string name, const bool value);

		void loadUniform(std::string name, const float value);

		void loadUniform(std::string name, const int value);

		void loadUniform(std::string name, colour *value);

		void loadUniform(std::string name, matrix2x2 *value);

		void loadUniform(std::string name, matrix3x3 *value);

		void loadUniform(std::string name, matrix4x4 *value);

		void loadUniform(std::string name, vector2 *value);

		void loadUniform(std::string name, vector3 *value);

		void loadUniform(std::string name, vector4 *value);

		/// <summary>
		/// Starts the shader program.
		/// </summary>
		void start();

		/// <summary>
		/// Stops the shader program.
		/// </summary>
		void stop();
	};
}