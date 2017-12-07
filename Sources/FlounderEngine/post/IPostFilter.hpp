#pragma once

#include "../models/Model.hpp"
#include "../renderer/pipelines/Pipeline.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a post effect shader and on application saves the result into a fbo.
	/// </summary>
	class IPostFilter
	{
	private:
		static const PipelineCreateInfo pipelineCreateInfo;
	protected:
		Pipeline *m_pipeline;
		Model *m_model;
	public:
		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="fragmentShader"> The fragment shader file. </param>
		IPostFilter(const std::string &fragmentShader, const int &subpass = 2, const std::vector<DescriptorType> &descriptors = {});

		/// <summary>
		/// Deconstructor for the post filter.
		/// </summary>
		virtual ~IPostFilter();

		/// <summary>
		/// Renders the filter.
		/// </summary>
		virtual void RenderFilter(const VkCommandBuffer *commandBuffer);
	protected:
		void CmdRender(const VkCommandBuffer *commandBuffer, const std::vector<VkWriteDescriptorSet> &descriptorWrites);
	};
}
