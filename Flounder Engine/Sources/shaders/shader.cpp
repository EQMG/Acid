#include "shader.h"

namespace flounder {
	shader::builder::builder()
	{
		m_shader = new shader();
	}

	shader::builder::~builder()
	{
	}

	shader::builder *shader::builder::addName(const std::string &name)
	{
		m_shader->m_name = name;
		return this;
	}

	shader::builder *shader::builder::addType(const shadertype &type)
	{
		m_shader->m_shaderTypes->push_back(type);
		return this;
	}

	flounder::shader *shader::builder::create()
	{
		// Creates the shader and loads it to the GPU.
		m_shader->m_programID = glCreateProgram();
		m_shader->loadTypes();

		m_shader->loadLocations();

		glLinkProgram(m_shader->m_programID);
		glValidateProgram(m_shader->m_programID);
		m_shader->deleteTypes();

		glUseProgram(m_shader->m_programID);
		m_shader->loadBindings();

		glUseProgram(0);

		delete this;
		return m_shader;
	}

	shader::shader()
	{
		m_name = "";
		m_shaderTypes = new std::vector<shadertype>();
		m_layoutLocations = new std::vector<std::string>();
		m_layoutBindings = new std::vector<std::string>();
		m_constants = new std::vector<std::pair<std::string, std::string>>();
		m_uniforms = new std::vector<std::string>();

		m_programID = NULL;
	}

	shader::~shader()
	{
		delete m_shaderTypes;
		delete m_layoutLocations;
		delete m_layoutBindings;
		delete m_constants;
		delete m_uniforms;

		glUseProgram(0);
		glDeleteProgram(m_programID);
	}

	shader::builder *shader::newShader()
	{
		return new builder();
	}

	void shader::start()
	{
		glUseProgram(m_programID);
	}

	void shader::stop()
	{
		glUseProgram(0);
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
		float *a = matrix2x2::toArray(*value);
		glUniformMatrix2fv(getUniform(name), 1, GL_FALSE, a);
		delete a;
	}

	void shader::loadUniform(const std::string &name, matrix3x3 *value)
	{
		float *a = matrix3x3::toArray(*value);
		glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, a);
		delete a;
	}

	void shader::loadUniform(const std::string &name, matrix4x4 *value)
	{
		float *a = matrix4x4::toArray(*value);
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

	void shader::loadTypes()
	{
		for (std::vector<shadertype>::iterator it = m_shaderTypes->begin(); it < m_shaderTypes->end(); it++)
		{
			loadType(&*it);
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

		if (helperstring::contains(line, "#version"))
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
		else if (helperstring::contains(line, "#include"))
		{
			std::string includeFile = helperstring::substring(line, 8 + 1, line.length() - 1); // "#include".length = 8
			includeFile = helperstring::replaceAll(includeFile, '\\s+');
			includeFile = helperstring::replaceAll(includeFile, '\"');

			std::string includeString = helperfile::readFile(includeFile);
			result = includeString;
		}
		else if (helperstring::startsWith(line, "layout"))
		{
			if (helperstring::contains(line, "location"))
			{
				m_layoutLocations->push_back(line);
				result = helperstring::trim(helperstring::substring(line, helperstring::findCharPos(line, ')') + 1, line.length()));
			}
			else if (helperstring::contains(line, "binding"))
			{
				m_layoutBindings->push_back(line);
				result = helperstring::trim(helperstring::substring(line, helperstring::findCharPos(line, ')') + 1, line.length()));
			}
		}
		else if (helperstring::startsWith(line, "const"))
		{
			std::string constString = line.substr(5 + 1, line.length() - 1);
			constString.erase(std::remove(constString.begin(), constString.end(), ';'), constString.end());

			std::string constType = helperstring::trim(constString.substr(0, helperstring::findCharPos(constString, ' ')));

			constString = helperstring::trim(constString.substr(helperstring::findCharPos(constString, ' '), constString.length()));

			std::vector<std::string> split = helperstring::split(constString, "=");

			std::string constName = helperstring::trim(split.at(0));
			std::string constValue = helperstring::trim(split.at(1));

			m_constants->push_back(std::pair<std::string, std::string>(constName, constValue));
		}
		else if (helperstring::startsWith(line, "uniform"))
		{
			std::string uniformString = helperstring::substring(line, 7 + 1, line.length() - 1); // "uniform".length = 7

			std::vector<std::string> split1 = helperstring::split(uniformString, "=");

			if (split1.size() != 0)
			{
				uniformString = split1.at(0);
			}

			std::vector<std::string> split = helperstring::split(uniformString, " ");

			std::string uniformType = helperstring::trim(split.at(0));
			std::string uniformName = helperstring::trim(split.at(1));

			if (uniformString.find("[") != std::string::npos && uniformString.find("]") != std::string::npos)
			{

				if (uniformName.find("[") != std::string::npos && uniformName.find("]") != std::string::npos)
				{
					uniformName = helperstring::trim(uniformName.substr(0, helperstring::findCharPos(uniformName, '[')));
				}

				std::string arraySize = helperstring::trim(uniformString.substr(helperstring::findCharPos(uniformString, '[') + 1, uniformString.length() - 1));
				arraySize.erase(std::remove(arraySize.begin(), arraySize.end(), ']'), arraySize.end());

				std::cout << line << " : " << uniformName << ":" << arraySize << std::endl;
			}

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
		for (std::vector<shadertype>::iterator it = m_shaderTypes->begin(); it < m_shaderTypes->end(); it++)
		{
			glDetachShader(m_programID, (*it).m_shaderID);
			glDeleteShader((*it).m_shaderID);
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
}