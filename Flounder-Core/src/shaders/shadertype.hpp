#pragma once

#include <string>

namespace flounder
{
	/// <summary>
	/// A class that represents a shader type.
	/// </summary>
	class shadertype
	{
	private:
		VkShaderStageFlagBits m_shaderFlag;
		std::string m_filePath;
	public:
		shadertype(VkShaderStageFlagBits shaderFlag, const std::string &filePath)
		{
			m_shaderFlag = shaderFlag;
			m_filePath = filePath;
		}

		VkShaderStageFlagBits getShaderFlag() const { return m_shaderFlag; }

		std::string getFilePath() const { return m_filePath; }
	};
}
