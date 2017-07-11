#include "shader.h"

namespace flounder {
	shader::shader(int n_args, ...)
	{
		m_shaderTypes = new std::vector<shadertype*>();
		m_layoutLocations = new std::vector<std::string>();
		m_layoutBindings = new std::vector<std::string>();
		m_uniforms = new std::map<std::string, std::vector<std::string>>();

		m_programID = glCreateProgram();

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			shadertype* type = va_arg(ap, shadertype*);
			type->m_string = "";

			std::string fileLoaded = helperfile::readFile(type->m_file);
			std::vector<std::string> lines = helperstring::split(fileLoaded, "\n");

			for (std::vector<std::string>::iterator it = lines.begin(); it < lines.end(); it++)
			{
				std::string line = helperstring::trim(*it);

				// TODO: Process.
				if (line.find("#version") != std::string::npos)
				{
					//if (MACOS)
					//{
					//	line = "#version 150 core";
					//}
					//else
					//{
					line = "#version 130";
					//}
				}
				else if (line.find("#include") != std::string::npos)
				{

				}
				else if (helperstring::startsWith(line, "layout"))
				{
					if (line.find("location") != std::string::npos)
					{
						m_layoutLocations->push_back(line);
						line = line.substr(helperstring::findCharPos(line, ')') + 1, line.length());
					}
					else if (line.find("binding") != std::string::npos)
					{
						m_layoutBindings->push_back(line);
						line = line.substr(helperstring::findCharPos(line, ')') + 1, line.length());
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
				}
				else if (helperstring::startsWith(line, "uniform"))
				{
					std::string uniformString = line.substr(7 + 1, line.length() - 1);
					uniformString.erase(std::remove(uniformString.begin(), uniformString.end(), ';'), uniformString.end());

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

				}

				type->m_string += line;
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
				std::cout << "Failed to compile vertex shader!" << std::endl << &error[0] << std::endl;
				glDeleteShader(type->m_shaderID);
				return;
			}

			glAttachShader(m_programID, type->m_shaderID);

			m_shaderTypes->push_back(type);
		}

		va_end(ap);

		glLinkProgram(m_programID);
		glValidateProgram(m_programID);

		glUseProgram(0);
	}

	shader::~shader()
	{
		delete m_shaderTypes;
		delete m_layoutLocations;
		delete m_layoutBindings;
		delete m_uniforms;
	}

	void shader::start()
	{
	}

	void shader::stop()
	{
	}
}