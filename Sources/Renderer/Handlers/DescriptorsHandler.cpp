#include "DescriptorsHandler.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
	DescriptorsHandler::DescriptorsHandler() :
		m_shader(nullptr),
		m_pushDescriptors(false),
		m_descriptorSet(nullptr),
		m_changed(false)
	{
	}

	DescriptorsHandler::DescriptorsHandler(const Pipeline &pipeline) :
		m_shader(pipeline.GetShaderProgram()),
		m_pushDescriptors(pipeline.IsPushDescriptors()),
		m_descriptorSet(std::make_unique<DescriptorSet>(pipeline)),
		m_changed(true)
	{
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, const Descriptor *descriptor, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shader == nullptr)
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
		int32_t location = m_shader->GetDescriptorLocation(descriptorName);

		if (location == -1)
		{
#if defined(ACID_VERBOSE)
			if (m_shader->ReportedNotFound(descriptorName, true))
			{
				Log::Error("Could not find descriptor in shader '%s' of name '%s'\n", m_shader->GetName().c_str(), descriptorName.c_str());
			}
#endif

			return;
		}

		// Adds the new descriptor value.
		m_descriptors.emplace(descriptorName, DescriptorValue{descriptor, offsetSize, static_cast<uint32_t>(location)});
		m_changed = true;
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, const Descriptor &descriptor, const std::optional<OffsetSize> &offsetSize)
	{
		Push(descriptorName, &descriptor, offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, const std::shared_ptr<Descriptor> &descriptor, const std::optional<OffsetSize> &offsetSize)
	{
		Push(descriptorName, descriptor.get(), offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shader == nullptr)
		{
			return;
		}

		uniformHandler.Update(m_shader->GetUniformBlock(descriptorName));
		Push(descriptorName, uniformHandler.GetUniformBuffer(), offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, StorageHandler &storageHandler, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shader == nullptr)
		{
			return;
		}

		storageHandler.Update(m_shader->GetUniformBlock(descriptorName));
		Push(descriptorName, storageHandler.GetStorageBuffer(), offsetSize);
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, PushHandler &pushHandler, const std::optional<OffsetSize> &offsetSize)
	{
		if (m_shader == nullptr)
		{
			return;
		}

		pushHandler.Update(m_shader->GetUniformBlock(descriptorName));
	}

	bool DescriptorsHandler::Update(const Pipeline &pipeline)
	{
		if (m_shader != pipeline.GetShaderProgram())
		{
			m_shader = pipeline.GetShaderProgram();
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
			m_writeDescriptors.clear();
			m_writeDescriptorSets.clear();

			for (const auto &[descriptorName, descriptor] : m_descriptors)
			{
				auto descriptorType = m_shader->GetDescriptorType(descriptor.location);

				if (!descriptorType)
				{
#if defined(ACID_VERBOSE)
					if (m_shader->ReportedNotFound(descriptorName, true))
					{
						Log::Error("Could not find descriptor in shader '%s' of name '%s' at location '%i'\n", 
							m_shader->GetName().c_str(), descriptorName.c_str(), descriptor.location);
					}
#endif
					continue;
				}

				auto writeDescriptor = descriptor.descriptor->GetWriteDescriptor(descriptor.location,
					*descriptorType, m_pushDescriptors ? VK_NULL_HANDLE : m_descriptorSet->GetDescriptorSet(), descriptor.offsetSize);
				m_writeDescriptorSets.emplace_back(writeDescriptor.GetWriteDescriptorSet());
				m_writeDescriptors.emplace_back(std::move(writeDescriptor));
			}

			if (!m_pushDescriptors)
			{
				m_descriptorSet->Update(m_writeDescriptorSets);
			}

			m_changed = false;
		}

		return true;
	}

	void DescriptorsHandler::BindDescriptor(const CommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		if (m_pushDescriptors)
		{
			auto logicalDevice = Renderer::Get()->GetLogicalDevice();

			Instance::FvkCmdPushDescriptorSetKHR(logicalDevice->GetLogicalDevice(), commandBuffer.GetCommandBuffer(), pipeline.GetPipelineBindPoint(), pipeline.GetPipelineLayout(), 
				0, static_cast<uint32_t>(m_writeDescriptorSets.size()), m_writeDescriptorSets.data());
		}
		else
		{
			m_descriptorSet->BindDescriptor(commandBuffer);
		}
	}
}
