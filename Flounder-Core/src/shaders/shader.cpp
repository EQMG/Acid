#include "shader.h"

namespace flounder
{
	shader::shader(const std::string &name, const int n_args, ...)
	{
		m_name = name;
		m_shaderTypes = new std::vector<shadertype>();
		m_layoutLocations = new std::vector<std::string>();
		m_layoutBindings = new std::vector<std::string>();
		m_constants = new std::vector<std::pair<std::string, std::string>>();
		m_uniforms = new std::vector<std::string>();

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_shaderTypes->push_back(va_arg(ap, shadertype));
		}

		va_end(ap);
#if 0
		m_programID = nullptr;
#endif
	}

	shader::~shader()
	{
		delete m_shaderTypes;
		delete m_layoutLocations;
		delete m_layoutBindings;
		delete m_constants;
		delete m_uniforms;

#if 0
		glUseProgram(0);
		glDeleteProgram(m_programID);
#endif
	}

	void shader::start()
	{
#if 0
		glUseProgram(m_programID);
#endif
	}

	void shader::stop()
	{
#if 0
		glUseProgram(0);
#endif
	}

	int shader::getUniform(const std::string &name)
	{
		return 0;
#if 0
		return glGetUniformLocation(m_programID, name.c_str());
#endif
	}

	void shader::loadUniform1i(const std::string &name, const bool &value)
	{
#if 0
		glUniform1i(getUniform(name), value);
#endif
	}

	void shader::loadUniform1i(const std::string &name, const int &value)
	{
#if 0
		glUniform1i(getUniform(name), value);
#endif
	}

	void shader::loadUniform1f(const std::string &name, const float &value)
	{
#if 0
		glUniform1f(getUniform(name), value);
#endif
	}

	void shader::loadUniform2f(const std::string &name, const float &x, const float &y)
	{
#if 0
		glUniform2f(getUniform(name), x, y);
#endif
	}

	void shader::loadUniform3f(const std::string &name, const float &x, const float &y, const float &z)
	{
#if 0
		glUniform3f(getUniform(name), x, y, z);
#endif
	}

	void shader::loadUniform4f(const std::string &name, const float &x, const float &y, const float &z, const float &w)
	{
#if 0
		glUniform4f(getUniform(name), x, y, z, w);
#endif
	}

	void shader::loadUniform2fv(const std::string &name, float value[4])
	{
#if 0
		glUniformMatrix2fv(getUniform(name), 1, GL_FALSE, value);
#endif
	}

	void shader::loadUniform3fv(const std::string &name, float value[9])
	{
#if 0
		glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, value);
#endif
	}

	void shader::loadUniform4fv(const std::string &name, float value[16])
	{
#if 0
		glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, value);
#endif
	}

	void shader::loadUniform2fv(const std::string &name, const matrix2x2 &value)
	{
		float *a = matrix2x2::toArray(value);
		loadUniform2fv(name, a);
		delete a;
	}

	void shader::loadUniform3fv(const std::string &name, const matrix3x3 &value)
	{
		float *a = matrix3x3::toArray(value);
		loadUniform3fv(name, a);
		delete a;
	}

	void shader::loadUniform4fv(const std::string &name, const matrix4x4 &value)
	{
		float *a = matrix4x4::toArray(value);
		loadUniform4fv(name, a);
		delete a;
	}
	
	void shader::loadUniform2f(const std::string &name, const vector2 &value)
	{
		loadUniform2f(name, value.m_x, value.m_y);
	}

	void shader::loadUniform3f(const std::string &name, const vector3 &value)
	{
		loadUniform3f(name, value.m_x, value.m_y, value.m_z);
	}

	void shader::loadUniform3f(const std::string &name, const colour &value)
	{
		loadUniform3f(name, value.m_r, value.m_g, value.m_b);
	}

	void shader::loadUniform4f(const std::string &name, const vector4 &value)
	{
		loadUniform4f(name, value.m_x, value.m_y, value.m_z, value.m_w);
	}

	void shader::loadUniform4f(const std::string &name, const colour &value)
	{
		loadUniform4f(name, value.m_r, value.m_g, value.m_b, value.m_a);
	}

	void shader::create()
	{
		for (shadertype type : *m_shaderTypes)
		{
			std::vector<char> shaderCode = readFile(type.m_filePath);
		}
	}

	std::vector<char> shader::readFile(const std::string &fileName)
	{
		std::ifstream file(fileName, std::ios::ate | std::ios::binary);

		if (!file.is_open()) 
		{
			throw std::runtime_error("Failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);		file.close();

		return buffer;
	}
}
