#include "ParticleType.hpp"

#include "Resources/Resources.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Helpers/String.hpp"
#include "Scenes/Scenes.hpp"
#include "Particle.hpp"

namespace acid
{
	const uint32_t ParticleType::INSTANCE_STEPS = 128;
	const float ParticleType::FRUSTUM_BUFFER = 1.4f;

	std::shared_ptr<ParticleType> ParticleType::Resource(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &stageCycles, const float &scale)
	{
		auto resource = Resources::Get()->Find(ToFilename(texture, numberOfRows, colourOffset, lifeLength, stageCycles, scale));

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
		m_maxInstances(0),
		m_instances(0),
		m_descriptorSet(DescriptorsHandler()),
		m_storageInstances(StorageHandler())
	{
	}

	void ParticleType::Update(const std::vector<Particle> &particles)
	{
		// Calculates a max instance count over the time of the type. TODO: Allow decreasing max using a timer and average count over the delay.
		uint32_t instances = INSTANCE_STEPS * static_cast<uint32_t>(std::ceil(static_cast<float>(particles.size()) / static_cast<float>(INSTANCE_STEPS)));
		m_maxInstances = std::max(m_maxInstances, instances);
		auto instanceDatas = std::vector<ParticleData>(m_maxInstances);
		m_instances = 0;

		for (auto &particle : particles)
		{
			if (!Scenes::Get()->GetCamera()->GetViewFrustum().SphereInFrustum(particle.GetPosition(), FRUSTUM_BUFFER * particle.GetScale()))
			{
				continue;
			}

			instanceDatas[m_instances] = GetInstanceData(particle);
			m_instances++;

			if (m_instances >= m_maxInstances)
			{
				break;
			}
		}

		m_storageInstances.Push(instanceDatas.data(), sizeof(ParticleData) * m_maxInstances);
	}

	bool ParticleType::CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene)
	{
		if (m_instances == 0)
		{
			return false;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("Instances", m_storageInstances);
		m_descriptorSet.Push("samplerColour", m_texture);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		// Draws the instanced objects.
		m_descriptorSet.BindDescriptor(commandBuffer);

		m_model->CmdRender(commandBuffer, m_instances);
		return true;
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

	std::string ParticleType::ToFilename(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &stageCycles, const float &scale)
	{
		std::stringstream result;
		result << "ParticleType_" << (texture == nullptr ? "nullptr" : texture->GetFilename()) << "_" << numberOfRows << "_" << colourOffset.GetHex() << "_" << lifeLength << "_" << stageCycles << "_" << scale;
		return result.str();
	}

	ParticleData ParticleType::GetInstanceData(const Particle &particle)
	{
		Matrix4 modelMatrix = Matrix4();
		modelMatrix = modelMatrix.Translate(particle.GetPosition());

		for (uint32_t i = 0; i < 3; i++)
		{
			modelMatrix[0][i] = particle.GetScale();
		}
		
		modelMatrix[1][0] = Maths::Radians(particle.GetRotation());

		Vector4 offsets = Vector4();
		offsets.m_x = particle.GetTextureOffset1().m_x;
		offsets.m_y = particle.GetTextureOffset1().m_y;
		offsets.m_z = particle.GetTextureOffset2().m_x;
		offsets.m_w = particle.GetTextureOffset2().m_y;

		Vector3 blend = Vector3();
		blend.m_x = particle.GetTextureBlendFactor();
		blend.m_y = particle.GetTransparency();
		blend.m_z = static_cast<float>(particle.GetParticleType()->GetNumberOfRows());

		ParticleData instanceData = {};
		instanceData.modelMatrix = modelMatrix;
		instanceData.colourOffset = particle.GetParticleType()->GetColourOffset();
		instanceData.offsets = offsets;
		instanceData.blend = blend;
		return instanceData;
	}
}
