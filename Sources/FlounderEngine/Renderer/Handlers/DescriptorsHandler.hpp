#pragma once

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

		void Push(const std::string &descriptorName, UniformHandler *uniformHandler);

		bool Update(const Pipeline &pipeline);

		DescriptorSet *GetDescriptorSet() const { return m_descriptorSet; }
	};
}