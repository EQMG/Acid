#pragma once

#include <string>
#include <vector>
#include "../../platforms/Platform.hpp"

namespace Flounder
{
	class ShaderStage
	{
	private:
		std::string m_filepath;
		VkShaderStageFlagBits m_shaderFlag;
	public:
		ShaderStage(const std::string &filepath, const VkShaderStageFlagBits &shaderFlag) :
			m_filepath(filepath),
			m_shaderFlag(shaderFlag)
		{
		}

		std::string GetFilepath() const { return m_filepath; }

		VkShaderStageFlagBits GetShaderFlag() const { return m_shaderFlag; }
	};

	struct ShaderStages
	{
	public:
		std::vector<ShaderStage> stages;

		static ShaderStages Create(const std::vector<ShaderStage> &types)
		{
			ShaderStages shaderStages = {};
			shaderStages.stages = std::vector<ShaderStage>(types);
			return shaderStages;
		}
	};
}
