#pragma once

#include <memory>
#include "Renderer/Descriptors/DescriptorSet.hpp"
#include "Renderer/Pipelines/ShaderProgram.hpp"
#include "UniformHandler.hpp"
#include "StorageHandler.hpp"
#include "PushHandler.hpp"

namespace acid
{
	/// <summary>
	/// Class that handles a descriptor set.
	/// </summary>
	class ACID_EXPORT DescriptorsHandler
	{
	private:
		ShaderProgram *m_shaderProgram;
		std::unique_ptr<DescriptorSet> m_descriptorSet;
		std::vector<IDescriptor *> m_descriptors;
		bool m_changed;
	public:
		DescriptorsHandler();

		explicit DescriptorsHandler(const IPipeline &pipeline);

		void Push(const std::string &descriptorName, IDescriptor *descriptor);

		void Push(const std::string &descriptorName, IDescriptor &descriptor) { Push(descriptorName, &descriptor); }

		void Push(const std::string &descriptorName, const std::shared_ptr<IDescriptor> &descriptor) { Push(descriptorName, descriptor.get()); }

		void Push(const std::string &descriptorName, UniformHandler *uniformHandler);

		void Push(const std::string &descriptorName, UniformHandler &uniformHandler) { Push(descriptorName, &uniformHandler); }

		void Push(const std::string &descriptorName, StorageHandler *storageHandler);

		void Push(const std::string &descriptorName, StorageHandler &storageHandler) { Push(descriptorName, &storageHandler); }

		void Push(const std::string &descriptorName, PushHandler *pushHandler);

		void Push(const std::string &descriptorName, PushHandler &pushHandler) { Push(descriptorName, &pushHandler); }

		bool Update(const IPipeline &pipeline);

		void BindDescriptor(const CommandBuffer &commandBuffer);

		DescriptorSet *GetDescriptorSet() const { return m_descriptorSet.get(); }
	};
}
