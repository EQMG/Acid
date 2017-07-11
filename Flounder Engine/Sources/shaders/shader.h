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

#include "shadertype.h"
#include "shaderbuilder.h"

namespace flounder {
	/// <summary>
	/// Class that represents a loaded shader.
	/// </summary>
	class shader {
	private:
		std::vector<shadertype*> *m_shaderTypes;
		std::vector<std::string> *m_layoutLocations;
		std::vector<std::string> *m_layoutBindings;
		std::vector<std::string> *m_uniforms;

		std::string m_name;
		GLuint m_programID;
	public:
		/// <summary>
		/// Creates a new shader.
		/// </summary>
		/// <param name="builder"> The shader builder to load the shader from. </param>
		shader(shaderbuilder *builder);

		/// <summary>
		/// Deconstructor for the shader.
		/// </summary>
		~shader();
	private:
		void loadTypes();

		void loadType(shadertype* type);

		std::string processLine(const std::string &line);

		void loadLocations();

		void deleteTypes();

		void loadBindings();
	public:
		/// <summary>
		/// Gets the loaded name for the shader.
		/// </summary>
		/// <returns> The shaders name. </returns>
		inline std::string getName() { return m_name; }

		/// <summary>
		/// Gets a uniform location from a name.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <returns> The uniform location. </returns>
		GLint getUniform(std::string name);

		/// <summary>
		/// Loads a boolean into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, const bool value);

		/// <summary>
		/// Loads a float into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, const float value);

		/// <summary>
		/// Loads a int/sampler2D into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, const int value);

		/// <summary>
		/// Loads a colour into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, colour *value);

		/// <summary>
		/// Loads a matrix 2x2 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, matrix2x2 *value);

		/// <summary>
		/// Loads a matrix 3x3 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, matrix3x3 *value);

		/// <summary>
		/// Loads a matrix 4x4 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, matrix4x4 *value);

		/// <summary>
		/// Loads a vector 2 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, vector2 *value);

		/// <summary>
		/// Loads a vector 3 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform(std::string name, vector3 *value);

		/// <summary>
		/// Loads a vector 4 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
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