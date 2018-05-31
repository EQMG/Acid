#pragma once

#include <memory>
#include "Renderer/Descriptors/DescriptorSet.hpp"
#include "UniformHandler.hpp"

namespace fl
{
	/// <summary>
	/// Class that handles a descriptor set.
	/// </summary>
	class FL_EXPORT DescriptorsHandler
	{
	private:
		ShaderProgram *m_shaderProgram;
		DescriptorSet *m_descriptorSet;
		std::vector<IDescriptor *> *m_descriptors;
		bool m_changed;
	public:
		DescriptorsHandler();

		~DescriptorsHandler();

		void Push(const std::string &descriptorName, IDescriptor *descriptor);

		void Push(const std::string &descriptorName, IDescriptor &descriptor) { Push(descriptorName, &descriptor); }

		void Push(const std::string &descriptorName, std::shared_ptr<IDescriptor> descriptor) { Push(descriptorName, descriptor.get()); }

		void Push(const std::string &descriptorName, UniformHandler *uniformHandler);

		void Push(const std::string &descriptorName, UniformHandler &uniformHandler) { Push(descriptorName, &uniformHandler); }

		void Push(const std::string &descriptorName, std::shared_ptr<UniformHandler> uniformHandler) { Push(descriptorName, uniformHandler.get()); }

		bool Update(const Pipeline &pipeline);

		void BindDescriptor(const CommandBuffer &commandBuffer) { m_descriptorSet->BindDescriptor(commandBuffer); }

		DescriptorSet *GetDescriptorSet() const { return m_descriptorSet; }
	};
}