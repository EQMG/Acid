#include "shader.h"

namespace flounder {
	shader::shader(shaderbuilder *builder)
	{
		m_name = builder->m_name;
		m_shaderTypes = builder->m_types;
		m_layoutLocations = new std::vector<std::string>();
		m_layoutBindings = new std::vector<std::string>();
		m_uniforms = new std::vector<std::string>();

		// Creates the shader and loads it to the GPU.
		m_programID = glCreateProgram();
		loadTypes();

		loadLocations();

		glLinkProgram(m_programID);
		glValidateProgram(m_programID);
		deleteTypes();

		glUseProgram(m_programID);
		loadBindings();

		glUseProgram(0);
	}

	shader::~shader()
	{
		delete m_shaderTypes;
		delete m_layoutLocations;
		delete m_layoutBindings;
		delete m_uniforms;

		glUseProgram(0);
		glDeleteProgram(m_programID);
	}

	void shader::loadTypes()
	{
		for (std::vector<shadertype*>::iterator it = m_shaderTypes->begin(); it < m_shaderTypes->end(); it++)
		{
			loadType(*it);
		}
	}

	void shader::loadType(shadertype* type)
	{
		type->m_string = "";

		std::string fileLoaded = helperfile::readFile(type->m_file);
		std::vector<std::string> lines = helperstring::split(fileLoaded, "\n");

		for (std::vector<std::string>::iterator it = lines.begin(); it < lines.end(); it++)
		{
			std::string line = helperstring::trim(*it);
			type->m_string += processLine(line) + "\n";
		}

		const char* source = type->m_string.c_str();
		type->m_shaderID = glCreateShader(type->m_shaderType);
		glShaderSource(type->m_shaderID, 1, &source, NULL);
		glCompileShader(type->m_shaderID);

		GLint result;
		glGetShaderiv(type->m_shaderID, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(type->m_shaderID, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(type->m_shaderID, length, &length, &error[0]);
			std::cout << "Failed to compile shader!" << std::endl << &error[0] << std::endl;
			glDeleteShader(type->m_shaderID);
			return;
		}

		glAttachShader(m_programID, type->m_shaderID);
	}

	std::string shader::processLine(const std::string &line)
	{
		std::string result = line;

		if (line.find("#version") != std::string::npos)
		{
			int major = display::get()->getGlfwMajor();
			int minor = display::get()->getGlfwMinor();

			if (major >= 3 && minor >= 2)
			{
				result = "#version 150 core";
			}
			else
			{
				result = "#version 130";
			}
		}
		else if (line.find("#include") != std::string::npos)
		{
			std::string includeFile = helperstring::substring(line, 8 + 1, line.length() - 1); // "#include".length = 8
			includeFile = helperstring::replaceAll(includeFile, '\\s+');
			includeFile = helperstring::replaceAll(includeFile, '\"');

			std::string includeString = helperfile::readFile(includeFile);
			result = includeString;
		}
		else if (helperstring::startsWith(line, "layout"))
		{
			if (line.find("location") != std::string::npos)
			{
				m_layoutLocations->push_back(line);
				result = helperstring::trim(helperstring::substring(line, helperstring::findCharPos(line, ')') + 1, line.length()));
			}
			else if (line.find("binding") != std::string::npos)
			{
				m_layoutBindings->push_back(line);
				result = helperstring::trim(helperstring::substring(line, helperstring::findCharPos(line, ')') + 1, line.length()));
			}
		}
		else if (helperstring::startsWith(line, "uniform"))
		{
			std::string uniformString = helperstring::substring(line, 7 + 1, line.length() - 1); // "uniform".length = 7

			std::vector<std::string> split = helperstring::split(uniformString, " ");

			std::string uniformType = helperstring::trim(split.at(0));
			std::string uniformName = helperstring::trim(split.at(1));

			m_uniforms->push_back(uniformName);
		}

		return result;
	}

	void shader::loadLocations()
	{
		for (std::vector<std::string>::iterator it = m_layoutLocations->begin(); it < m_layoutLocations->end(); it++)
		{
			std::string locationName = helperstring::substring(*it, (*it).find_last_of(" ") + 1, (*it).length() - 1);
			std::string locationType = helperstring::substring(*it, 0, (*it).find_last_of(" ") + 1);
			locationType = helperstring::substring(locationType, (*it).find_last_of(")") + 1, locationType.length());
			locationType = helperstring::trim(locationType);

			std::string value = helperstring::substring(*it, helperstring::findCharPos((*it), '=') + 1, helperstring::findCharPos((*it), ')'));
			value = helperstring::replaceAll(value, '\\s+');
			value = helperstring::trim(value);
			int index = atoi(value.c_str());

			if (locationType.find("in") != std::string::npos)
			{
				glBindAttribLocation(m_programID, index, locationName.c_str());
			}
			else if (locationType.find("out") != std::string::npos)
			{
				glBindFragDataLocation(m_programID, index, locationName.c_str());
			}
			else
			{
				std::cout << "Could not find location type of: " << locationType << std::endl;
			}
		}
	}

	void shader::deleteTypes()
	{
		for (std::vector<shadertype*>::iterator it = m_shaderTypes->begin(); it < m_shaderTypes->end(); it++)
		{
			glDetachShader(m_programID, (*it)->m_shaderID);
			glDeleteShader((*it)->m_shaderID);
		}
	}

	void shader::loadBindings()
	{
		for (std::vector<std::string>::iterator it = m_layoutBindings->begin(); it < m_layoutBindings->end(); it++)
		{
			std::string bindingName = helperstring::substring(*it, (*it).find_last_of(" ") + 1, (*it).length() - 1);

			std::string value = helperstring::substring(*it, helperstring::findCharPos((*it), '=') + 1, helperstring::findCharPos((*it), ')'));
			value = helperstring::replaceAll(value, '\\s+');
			value = helperstring::trim(value);
			int index = atoi(value.c_str());

			int location = glGetUniformLocation(m_programID, bindingName.c_str());
			glUniform1i(location, index);
		}
	}

	GLint shader::getUniform(const std::string &name)
	{
		return glGetUniformLocation(m_programID, name.c_str());
	}

	void shader::loadUniform(const std::string &name, const bool value)
	{
		glUniform1i(getUniform(name), value);
	}

	void shader::loadUniform(const std::string &name, const float value)
	{
		glUniform1f(getUniform(name), value);
	}

	void shader::loadUniform(const std::string &name, const int value)
	{
		glUniform1i(getUniform(name), value);
	}

	void shader::loadUniform(const std::string &name, colour *value)
	{
		glUniform4f(getUniform(name), value->r, value->g, value->b, value->a);
	}

	void shader::loadUniform(const std::string &name, matrix2x2 *value)
	{
		float *a = matrix2x2::toArray(value);
		glUniformMatrix2fv(getUniform(name), 1, GL_FALSE, a);
		delete a;
	}

	void shader::loadUniform(const std::string &name, matrix3x3 *value)
	{
		float *a = matrix3x3::toArray(value);
		glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, a);
		delete a;
	}

	void shader::loadUniform(const std::string &name, matrix4x4 *value)
	{
		float *a = matrix4x4::toArray(value);
		glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, a);
		delete a;
	}

	void shader::loadUniform(const std::string &name, vector2 *value)
	{
		glUniform2f(getUniform(name), value->x, value->y);
	}

	void shader::loadUniform(const std::string &name, vector3 *value)
	{
		glUniform3f(getUniform(name), value->x, value->y, value->z);
	}

	void shader::loadUniform(const std::string &name, vector4 *value)
	{
		glUniform4f(getUniform(name), value->x, value->y, value->z, value->w);
	}

	void shader::start()
	{
		glUseProgram(m_programID);
	}

	void shader::stop()
	{
		glUseProgram(0);
	}
}