#pragma once

#include "Helpers/TypeTraits.hpp"
#include "Renderer/Descriptors/DescriptorSet.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Buffers/StorageHandler.hpp"
#include "Renderer/Buffers/PushHandler.hpp"
#include "Renderer/Pipelines/Shader.hpp"

namespace acid
{
/**
 * Class that handles a descriptor set.
 */
class ACID_EXPORT DescriptorsHandler
{
public:
	DescriptorsHandler();

	explicit DescriptorsHandler(const Pipeline &pipeline);

	template<typename T>
	void Push(const std::string &descriptorName, const T &descriptor, const std::optional<OffsetSize> &offsetSize = {})
	{
		if (m_shader == nullptr)
		{
			return;
		}

		// Finds the local value given to the descriptor name.
		auto it = m_descriptors.find(descriptorName);

		if (it != m_descriptors.end())
		{
			// If the descriptor and size have not changed then the write is not modified.
			if (it->second.m_descriptor == TypeTraits::AsPtr(descriptor) && it->second.m_offsetSize == offsetSize)
			{
				return;
			}

			m_descriptors.erase(it);
		}

		// Only non-null descriptors can be mapped.
		if (TypeTraits::AsPtr(descriptor) == nullptr)
		{
			return;
		}

		// When adding the descriptor find the location in the shader.
		auto location = m_shader->GetDescriptorLocation(descriptorName);

		if (!location)
		{
#if defined(ACID_VERBOSE)
			if (m_shader->ReportedNotFound(descriptorName, true))
			{
				Log::Error("Could not find descriptor in shader '%s' of name '%s'\n", m_shader->GetName().c_str(), descriptorName.c_str());
			}
#endif

			return;
		}

		auto descriptorType = m_shader->GetDescriptorType(*location);

		if (!descriptorType)
		{
#if defined(ACID_VERBOSE)
			if (m_shader->ReportedNotFound(descriptorName, true))
			{
				Log::Error("Could not find descriptor in shader '%s' of name '%s' at location '%i'\n", m_shader->GetName().c_str(), descriptorName.c_str(), *location);
			}
#endif
			return;
		}

		// Adds the new descriptor value.
		auto writeDescriptor = TypeTraits::AsPtr(descriptor)->GetWriteDescriptor(*location, *descriptorType, offsetSize);
		m_descriptors.emplace(descriptorName, DescriptorValue{ TypeTraits::AsPtr(descriptor), std::move(writeDescriptor), offsetSize, *location });
		m_changed = true;
	}

	template<typename T>
	void Push(const std::string &descriptorName, const T &descriptor, WriteDescriptorSet writeDescriptorSet)
	{
		if (m_shader == nullptr)
		{
			return;
		}

		auto it = m_descriptors.find(descriptorName);

		if (it != m_descriptors.end())
		{
			m_descriptors.erase(it);
		}

		auto location = m_shader->GetDescriptorLocation(descriptorName);
		auto descriptorType = m_shader->GetDescriptorType(*location);

		m_descriptors.emplace(descriptorName, DescriptorValue{ TypeTraits::AsPtr(descriptor), std::move(writeDescriptorSet), {}, *location });
		m_changed = true;
	}

	void Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize = {});

	void Push(const std::string &descriptorName, StorageHandler &storageHandler, const std::optional<OffsetSize> &offsetSize = {});

	void Push(const std::string &descriptorName, PushHandler &pushHandler, const std::optional<OffsetSize> &offsetSize = {});

	bool Update(const Pipeline &pipeline);

	void BindDescriptor(const CommandBuffer &commandBuffer, const Pipeline &pipeline);

	const DescriptorSet *GetDescriptorSet() const { return m_descriptorSet.get(); }

private:
	struct DescriptorValue
	{
		const Descriptor *m_descriptor;
		WriteDescriptorSet m_writeDescriptor;
		std::optional<OffsetSize> m_offsetSize;
		uint32_t m_location;
	};

	const Shader *m_shader;
	bool m_pushDescriptors;
	std::unique_ptr<DescriptorSet> m_descriptorSet;

	std::map<std::string, DescriptorValue> m_descriptors;
	std::vector<VkWriteDescriptorSet> m_writeDescriptorSets;
	bool m_changed;
};
}
