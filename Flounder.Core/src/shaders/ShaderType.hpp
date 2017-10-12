#pragma once

#include <string>

namespace Flounder
{
	/// <summary>
	/// A class that represents a shader type.
	/// </summary>
	class ShaderType
	{
	private:
		VkShaderStageFlagBits m_shaderFlag;
		std::string m_filename;
	public:
		ShaderType(const VkShaderStageFlagBits &shaderFlag, const std::string &filename)
		{
			m_shaderFlag = shaderFlag;
			m_filename = filename;
		}

		VkShaderStageFlagBits GetShaderFlag() const { return m_shaderFlag; }

		std::string GetFilename() const { return m_filename; }
	};
}
