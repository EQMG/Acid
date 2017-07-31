#pragma once

#include <string>
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

namespace flounder
{
	/// <summary>
	/// Class that represents a loaded shader.
	/// </summary>
	class shader
	{
		/// <summary>
		/// A builder used to set shader parameters for loading.
		/// </summary>
		class builder
		{
		private:
			shader *m_shader;
		public:
			/// <summary>
			/// Creates a new shader builder.
			/// </summary>
			/// <param name="name"> The name. </param>
			builder();

			/// <summary>
			/// Deconstructor for the shader builder.
			/// </summary>
			~builder();

			/// <summary>
			/// Sets the name of the shader.
			/// </summary>
			/// <param name="name"> The shaders name. </param>
			/// <returns> This. </returns>
			builder *addName(const std::string &name);

			/// <summary>
			/// Adds a new shader type to the load pool.
			/// </summary>
			/// <param name="type"> The shader type to add. </param>
			/// <returns> This. </returns>
			builder *addType(const shadertype &type);

			/// <summary>
			/// Creates a shader from the builder.
			/// </summary>
			/// <returns> The created shader. </returns>
			shader *create();
		};

	protected:
		builder *m_builder;

		std::string m_name;
		std::vector<shadertype> *m_shaderTypes;
		std::vector<std::string> *m_layoutLocations;
		std::vector<std::string> *m_layoutBindings;
		std::vector<std::pair<std::string, std::string>> *m_constants;
		std::vector<std::string> *m_uniforms;

		GLuint m_programID;

		/// <summary>
		/// Creates a new shader.
		/// </summary>
		/// <param name="builder"> The shaders builder. </param>
		shader(builder *builder);
	public:
		/// <summary>
		/// Deconstructor for the shader.
		/// </summary>
		~shader();

		/// <summary>
		/// Creates a new shader builder that is used to configure a shader.
		/// </summary>
		/// <returns> The shader builder. </returns>
		static builder *newShader();

		/// <summary>
		/// Gets the loaded name for the shader.
		/// </summary>
		/// <returns> The shaders name. </returns>
		inline std::string &getName() { return m_name; }

		/// <summary>
		/// Starts the shader program.
		/// </summary>
		void start();

		/// <summary>
		/// Stops the shader program.
		/// </summary>
		void stop();

		/// <summary>
		/// Gets a uniform location from a name.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <returns> The uniform location. </returns>
		GLint getUniform(const std::string &name);

		/// <summary>
		/// Loads a boolean into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform1i(const std::string &name, const bool &value);

		/// <summary>
		/// Loads a int/sampler2D into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform1i(const std::string &name, const int &value);

		/// <summary>
		/// Loads a float into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform1f(const std::string &name, const float &value);

		/// <summary>
		/// Loads a vector 2 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="x"> The x value to load into the uniform. </param>
		/// <param name="y"> The y value to load into the uniform. </param>
		void loadUniform2f(const std::string &name, const float &x, const float &y);

		/// <summary>
		/// Loads a vector 3 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="x"> The x value to load into the uniform. </param>
		/// <param name="y"> The y value to load into the uniform. </param>
		/// <param name="z"> The z value to load into the uniform. </param>
		void loadUniform3f(const std::string &name, const float &x, const float &y, const float &z);

		/// <summary>
		/// Loads a vector 4 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="x"> The x value to load into the uniform. </param>
		/// <param name="y"> The y value to load into the uniform. </param>
		/// <param name="z"> The z value to load into the uniform. </param>
		/// <param name="w"> The w value to load into the uniform. </param>
		void loadUniform4f(const std::string &name, const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Loads a matrix 2x2 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value array to load into the uniform. </param>
		void loadUniform2fv(const std::string &name, float value[4]);

		/// <summary>
		/// Loads a matrix 3x3 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value array to load into the uniform. </param>
		void loadUniform3fv(const std::string &name, float value[9]);

		/// <summary>
		/// Loads a matrix 4x4 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value array to load into the uniform. </param>
		void loadUniform4fv(const std::string &name, float value[16]);

		/// <summary>
		/// Loads a matrix 2x2 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform2fv(const std::string &name, const matrix2x2 &value);

		/// <summary>
		/// Loads a matrix 3x3 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform3fv(const std::string &name, const matrix3x3 &value);

		/// <summary>
		/// Loads a matrix 4x4 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform4fv(const std::string &name, const matrix4x4 &value);

		/// <summary>
		/// Loads a vector 2 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform2f(const std::string &name, const vector2 &value);

		/// <summary>
		/// Loads a vector 3 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform3f(const std::string &name, const vector3 &value);

		/// <summary>
		/// Loads a colour into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform3f(const std::string &name, const colour &value);

		/// <summary>
		/// Loads a vector 4 into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform4f(const std::string &name, const vector4 &value);

		/// <summary>
		/// Loads a colour into a uniform.
		/// </summary>
		/// <param name="name"> The uniforms name. </param>
		/// <param name="value"> The value to load into the uniform. </param>
		void loadUniform4f(const std::string &name, const colour &value);
	private:
		/// <summary>
		/// Loads all of the types provided.
		/// </summary>
		void loadTypes();

		/// <summary>
		/// Loads the type into memory.
		/// </summary>
		/// <param name="type"> The type. </param>
		void loadType(shadertype *type);

		/// <summary>
		/// Processes the shaders line looking for keywords and optimizing for platforms.
		/// </summary>
		/// <param name="line"> The line to process. </param>
		/// <returns> The processed line. </returns>
		std::string processLine(const std::string &line);

		/// <summary>
		/// Loads shader locations to make shader usage more simple.
		/// </summary>
		void loadLocations();

		/// <summary>
		/// Clears the loaded types from memory as they are not needed after creating the shader.
		/// </summary>
		void deleteTypes();

		/// <summary>
		/// Loads shader binds to make shader usage more simple.
		/// </summary>
		void loadBindings();
	};
}
