#include "ParticleType.hpp"

#include "Resources/Resources.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Helpers/String.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	const uint32_t ParticleType::MAX_TYPE_INSTANCES = 512;

	std::shared_ptr<ParticleType> ParticleType::Resource(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &stageCycles, const float &scale)
	{
		auto resource = Resources::Get()->Get(ToFilename(texture, numberOfRows, colourOffset, lifeLength, stageCycles, scale));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ParticleType>(resource);
		}

		auto result = std::make_shared<ParticleType>(texture, numberOfRows, colourOffset, lifeLength, stageCycles, scale);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<ParticleType> ParticleType::Resource(const std::string &data)
	{
		auto split = String::Split(data, "_");
		auto texture = Texture::Resource(split[1]);
		uint32_t numberOfRows = String::From<uint32_t>(split[2]);
		Colour colourOffset = Colour(split[3]);
		float lifeLength = String::From<float>(split[4]);
		float stageCycles = String::From<float>(split[5]);
		float scale = String::From<float>(split[6]);
		return Resource(texture, numberOfRows, colourOffset, lifeLength, stageCycles, scale);
	}

	ParticleType::ParticleType(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &stageCycles, const float &scale) :
		m_filename(ToFilename(texture, numberOfRows, colourOffset, lifeLength, stageCycles, scale)),
		m_texture(texture),
		m_model(ModelRectangle::Resource(-0.5f, 0.5f)),
		m_numberOfRows(numberOfRows),
		m_colourOffset(colourOffset),
		m_lifeLength(lifeLength),
		m_stageCycles(stageCycles),
		m_scale(scale),
		m_storageBuffer(StorageHandler()),
		m_descriptorSet(DescriptorsHandler())
	{
	}

	void ParticleType::Decode(const Metadata &metadata)
	{
		m_texture = Texture::Resource(metadata.GetChild<std::string>("Texture"));
		m_numberOfRows = metadata.GetChild<uint32_t>("Number Of Rows");
		m_colourOffset = metadata.GetChild<Colour>("Colour Offset");
		m_lifeLength = metadata.GetChild<float>("Life Length");
		m_stageCycles = metadata.GetChild<float>("Stage Cycles");
		m_scale = metadata.GetChild<float>("Scale");
		m_filename = ToFilename(m_texture, m_numberOfRows, m_colourOffset, m_lifeLength, m_stageCycles, m_scale);
	}

	void ParticleType::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Texture", m_texture == nullptr ? "" : m_texture->GetFilename());
		metadata.SetChild<uint32_t>("Number Of Rows", m_numberOfRows);
		metadata.SetChild<Colour>("Colour Offset", m_colourOffset);
		metadata.SetChild<float>("Life Length", m_lifeLength);
		metadata.SetChild<float>("Stage Cycles", m_stageCycles);
		metadata.SetChild<float>("Scale", m_scale);
	}

	bool ParticleType::CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene, const std::vector<ParticleData> &instanceData)
	{
		m_storageBuffer.Push("data", *instanceData.data(), sizeof(ParticleData) * MAX_TYPE_INSTANCES);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("Instances", m_storageBuffer);
		m_descriptorSet.Push("samplerColour", m_texture);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		// Draws the instanced objects.
		m_descriptorSet.BindDescriptor(commandBuffer);

		m_model->CmdRender(commandBuffer, static_cast<uint32_t>(instanceData.size()));
		return true;
	}

	std::string ParticleType::ToFilename(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &stageCycles, const float &scale)
	{
		std::stringstream result;
		result << "ParticleType_" << (texture == nullptr ? "nullptr" : texture->GetFilename()) << "_" << numberOfRows << "_" << colourOffset.GetHex() << "_" << lifeLength << "_" << stageCycles << "_" << scale;
		return result.str();
	}
}
