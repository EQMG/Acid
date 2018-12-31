#include "DescriptorsHandler.hpp"

#include "Display/Display.hpp"

namespace acid
{
	DescriptorsHandler::DescriptorsHandler() :
		m_shaderProgram(nullptr),
		m_pushDescriptors(false),
		m_descriptors(std::map<std::string, DescriptorValue>()),
		m_descriptorWrites(std::vector<WriteDescriptorSet>()),
		m_descriptorSet(nullptr),
		m_changed(false)
	{
	}

	DescriptorsHandler::DescriptorsHandler(const IPipeline &pipeline) :
		m_shaderProgram(pipeline.GetShaderProgram()),
		m_pushDescriptors(pipeline.IsPushDescriptors()),
		m_descriptors(std::map<std::string, DescriptorValue>()),
		m_descriptorWrites(std::vector<WriteDescriptorSet>()),
		m_descriptorSet(std::make_unique<DescriptorSet>(pipeline)),
		m_changed(true)
	{
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, Descriptor *descriptor, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

		// Finds the local value given to the descriptor name.
		auto it = m_descriptors.find(descriptorName);

		if (it != m_descriptors.end())
		{
			// If the descriptor or size has changed the descriptor values are reset.
			if (it->second.descriptor != descriptor || it->second.offsetSize != offsetSize)
			{
				m_descriptors.erase(it);
			}
			else
			{
				return;
			}
		}

		// When adding the descriptor find the location in the shader.
		int32_t location = m_shaderProgram->GetDescriptorLocation(descriptorName);

		if (location == -1)
		{
#if defined(ACID_VERBOSE)
			if (m_shaderProgram->ReportedNotFound(descriptorName, true))
			{
				Log::Error("Could not find descriptor in shader '%s' of name '%s'\n", m_shaderProgram->GetName().c_str(), descriptorName.c_str());
			}
#endif

			return;
		}

		// Adds the new descriptor value.
		m_descriptors.emplace(descriptorName, DescriptorValue{descriptor, offsetSize, static_cast<uint32_t>(location)});
		m_changed = true;
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, Descriptor &descriptor, const std::optional<OffsetSize> &offsetSize)
	{
		Push(descriptorName, &descriptor, offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, const std::shared_ptr<Descriptor> &descriptor, const std::optional<OffsetSize> &offsetSize)
	{
		Push(descriptorName, descriptor.get(), offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

		uniformHandler.Update(m_shaderProgram->GetUniformBlock(descriptorName));
		Push(descriptorName, uniformHandler.GetUniformBuffer(), offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, StorageHandler &storageHandler, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

		storageHandler.Update(m_shaderProgram->GetUniformBlock(descriptorName));
		Push(descriptorName, storageHandler.GetStorageBuffer(), offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, PushHandler &pushHandler, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

		pushHandler.Update(m_shaderProgram->GetUniformBlock(descriptorName));
	}

	bool DescriptorsHandler::Update(const IPipeline &pipeline)
	{
		if (m_shaderProgram != pipeline.GetShaderProgram())
		{
			m_shaderProgram = pipeline.GetShaderProgram();
			m_pushDescriptors = pipeline.IsPushDescriptors();
			m_descriptors.clear();

			if (!m_pushDescriptors)
			{
				m_descriptorSet = std::make_unique<DescriptorSet>(pipeline);
			}

			m_changed = false;
			return false;
		}

		if (m_changed)
		{
			m_descriptorWrites.clear();

			for (auto &[descriptorName, descriptor] : m_descriptors)
			{
				VkDescriptorType descriptorType = m_shaderProgram->GetDescriptorType(descriptor.location);
				m_descriptorWrites.emplace_back(descriptor.descriptor->GetWriteDescriptor(descriptor.location,
					descriptorType, m_pushDescriptors ? nullptr : m_descriptorSet->GetDescriptorSet(), descriptor.offsetSize));
			}

			if (!m_pushDescriptors)
			{
				m_descriptorSet->Update(m_descriptorWrites);
			}

			m_changed = false;
		}

		return true;
	}

	void DescriptorsHandler::BindDescriptor(const CommandBuffer &commandBuffer, const IPipeline &pipeline)
	{
		if (m_pushDescriptors)
		{
			auto logicalDevice = Display::Get()->GetLogicalDevice();

			std::vector<VkWriteDescriptorSet> descriptors = {}; // TODO: Remove.

			for (auto &descriptorWrite : m_descriptorWrites)
			{
				auto descriptor = static_cast<VkWriteDescriptorSet>(descriptorWrite);
				descriptor.pImageInfo = &descriptorWrite.imageInfo;
				descriptor.pBufferInfo = &descriptorWrite.bufferInfo;
				descriptors.emplace_back(descriptor);
			}

			Display::FvkCmdPushDescriptorSetKHR(logicalDevice, commandBuffer.GetCommandBuffer(), pipeline.GetPipelineBindPoint(), pipeline.GetPipelineLayout(),
				0, static_cast<uint32_t>(descriptors.size()), descriptors.data());
		}
		else
		{
			m_descriptorSet->BindDescriptor(commandBuffer);
		}
	}
}
