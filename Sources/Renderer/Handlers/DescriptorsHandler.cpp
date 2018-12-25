#include "DescriptorsHandler.hpp"

namespace acid
{
	DescriptorsHandler::DescriptorsHandler() :
		m_shaderProgram(nullptr),
		m_descriptors(std::map<std::string, DescriptorValue>()),
		m_descriptorSet(nullptr),
		m_changed(false)
	{
	}

	DescriptorsHandler::DescriptorsHandler(const IPipeline &pipeline) :
		m_shaderProgram(pipeline.GetShaderProgram()),
		m_descriptors(std::map<std::string, DescriptorValue>()),
		m_descriptorSet(std::make_unique<DescriptorSet>(pipeline)),
		m_changed(true)
	{
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, IDescriptor *descriptor, const std::optional<OffsetSize> &offsetSize)
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

	void DescriptorsHandler::Push(const std::string &descriptorName, IDescriptor &descriptor, const std::optional<OffsetSize> &offsetSize)
	{
		Push(descriptorName, &descriptor, offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, const std::shared_ptr<IDescriptor> &descriptor, const std::optional<OffsetSize> &offsetSize)
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
			m_descriptors.clear();
			m_descriptorSet = std::make_unique<DescriptorSet>(pipeline);
			m_changed = false;
			return false;
		}

		if (m_changed)
		{
			std::vector<WriteDescriptorSet> descriptorWrites = {};

			for (auto &[descriptorName, descriptor] : m_descriptors)
			{
				VkDescriptorType descriptorType = m_shaderProgram->GetDescriptorType(descriptor.location);
				descriptorWrites.emplace_back(descriptor.descriptor->GetWriteDescriptor(descriptor.location,
					descriptorType, *m_descriptorSet, descriptor.offsetSize));
			}

			m_descriptorSet->Update(descriptorWrites);
			m_changed = false;
		}

		return true;
	}

	void DescriptorsHandler::BindDescriptor(const CommandBuffer &commandBuffer)
	{
		m_descriptorSet->BindDescriptor(commandBuffer);
	}
}
