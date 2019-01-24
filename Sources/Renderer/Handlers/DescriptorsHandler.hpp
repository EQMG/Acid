#pragma once

#include <map>
#include <memory>
#include <optional>
#include "Renderer/Descriptors/DescriptorSet.hpp"
#include "Renderer/Pipelines/Shader.hpp"
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
	public:
		DescriptorsHandler();

		explicit DescriptorsHandler(const Pipeline &pipeline);

		void Push(const std::string &descriptorName, const Descriptor *descriptor, const std::optional<OffsetSize> &offsetSize = {});

		void Push(const std::string &descriptorName, const Descriptor &descriptor, const std::optional<OffsetSize> &offsetSize = {});

		void Push(const std::string &descriptorName, const std::shared_ptr<Descriptor> &descriptor, const std::optional<OffsetSize> &offsetSize = {});

		void Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize = {});

		void Push(const std::string &descriptorName, StorageHandler &storageHandler, const std::optional<OffsetSize> &offsetSize = {});

		void Push(const std::string &descriptorName, PushHandler &pushHandler, const std::optional<OffsetSize> &offsetSize = {});

		bool Update(const Pipeline &pipeline);

		void BindDescriptor(const CommandBuffer &commandBuffer, const Pipeline &pipeline);

		const DescriptorSet *GetDescriptorSet() const { return m_descriptorSet.get(); }
	private:
		struct DescriptorValue
		{
			const Descriptor *descriptor;
			std::optional<OffsetSize> offsetSize;
			uint32_t location;
		};

		const Shader *m_shader;
		bool m_pushDescriptors;
		std::map<std::string, DescriptorValue> m_descriptors;
		std::vector<WriteDescriptorSet> m_writeDescriptors;
		std::vector<VkWriteDescriptorSet> m_writeDescriptorSets;
		std::unique_ptr<DescriptorSet> m_descriptorSet;
		bool m_changed;
	};
}
