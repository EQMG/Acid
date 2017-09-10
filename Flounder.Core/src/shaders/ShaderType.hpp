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
		std::string m_filePath;
	public:
		ShaderType(VkShaderStageFlagBits shaderFlag, const std::string &filePath)
		{
			m_shaderFlag = shaderFlag;
			m_filePath = filePath;
		}

		VkShaderStageFlagBits GetShaderFlag() const { return m_shaderFlag; }

		std::string GetFilePath() const { return m_filePath; }
	};
}
