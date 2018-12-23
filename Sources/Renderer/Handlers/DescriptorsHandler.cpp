#include "DescriptorsHandler.hpp"

namespace acid
{
	DescriptorsHandler::DescriptorsHandler() :
		m_shaderProgram(nullptr),
		m_descriptorSet(nullptr),
		m_descriptors(std::vector<IDescriptor *>()),
		m_offsetSizes(std::vector<std::optional<OffsetSize>>()),
		m_changed(false)
	{
	}

	DescriptorsHandler::DescriptorsHandler(const IPipeline &pipeline) :
		m_shaderProgram(pipeline.GetShaderProgram()),
		m_descriptorSet(std::make_unique<DescriptorSet>(pipeline)),
		m_descriptors(std::vector<IDescriptor *>(m_shaderProgram->GetLastDescriptorBinding() + 1)),
		m_offsetSizes(std::vector<std::optional<OffsetSize>>(m_shaderProgram->GetLastDescriptorBinding() + 1)),
		m_changed(true)
	{
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, IDescriptor *descriptor, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

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

		if (m_descriptors.at(location) != descriptor)
		{
			m_descriptors.at(location) = descriptor;
			m_offsetSizes.at(location) = offsetSize;
			m_changed = true;
		}
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

		pushHandler.Update(m_shaderProgram->GetUniformBlock(descriptorName), offsetSize);
	}

	bool DescriptorsHandler::Update(const IPipeline &pipeline)
	{
		if (m_shaderProgram != pipeline.GetShaderProgram())
		{
			m_descriptors.clear();

			m_shaderProgram = pipeline.GetShaderProgram();
			m_descriptors.resize(m_shaderProgram->GetLastDescriptorBinding() + 1);
			m_offsetSizes.resize(m_shaderProgram->GetLastDescriptorBinding() + 1);
			m_descriptorSet = std::make_unique<DescriptorSet>(pipeline);
			m_changed = false;
			return false;
		}

		if (m_changed)
		{
			std::vector<WriteDescriptorSet> descriptorWrites = {};

			for (uint32_t i = 0; i < m_descriptors.size(); i++)
			{
				if (m_descriptors.at(i) != nullptr)
				{
					VkDescriptorType descriptorType = m_shaderProgram->GetDescriptorType(i);
					descriptorWrites.emplace_back(m_descriptors[i]->GetWriteDescriptor(i, descriptorType, *m_descriptorSet, m_offsetSizes[i]));
				}
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
