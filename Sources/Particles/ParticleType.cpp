#include "ParticleType.hpp"

#include "Resources/Resources.hpp"
#include "Maths/Maths.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Helpers/String.hpp"
#include "Scenes/Scenes.hpp"
#include "Particle.hpp"

namespace acid
{
	static const uint32_t INSTANCE_STEPS = 128;
	static const float FRUSTUM_BUFFER = 1.4f;

	std::shared_ptr<ParticleType> ParticleType::Create(const Metadata &metadata)
	{
		auto result = std::make_shared<ParticleType>(nullptr);
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<ParticleType> ParticleType::Create(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &stageCycles, const float &scale)
	{
		auto temp = ParticleType(texture, numberOfRows, colourOffset, lifeLength, stageCycles, scale);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	ParticleType::ParticleType(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &stageCycles, const float &scale) :
		m_texture(texture),
		m_model(ModelRectangle::Create(-0.5f, 0.5f)),
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
		std::vector<ParticleTypeData> instanceDatas(m_maxInstances);
		m_instances = 0;

		for (const auto &particle : particles)
		{
			if (!Scenes::Get()->GetCamera()->GetViewFrustum().SphereInFrustum(particle.GetPosition(), FRUSTUM_BUFFER * particle.GetScale()))
			{
				continue;
			}

			Matrix4 modelMatrix = Matrix4();
			modelMatrix = modelMatrix.Translate(particle.GetPosition());

			for (uint32_t i = 0; i < 3; i++)
			{
				modelMatrix[0][i] = particle.GetScale();
			}

			modelMatrix[1][0] = particle.GetRotation() * Maths::DegToRad;

			Vector4 offsets = Vector4();
			offsets.m_x = particle.GetTextureOffset1().m_x;
			offsets.m_y = particle.GetTextureOffset1().m_y;
			offsets.m_z = particle.GetTextureOffset2().m_x;
			offsets.m_w = particle.GetTextureOffset2().m_y;

			Vector3 blend = Vector3();
			blend.m_x = particle.GetTextureBlendFactor();
			blend.m_y = particle.GetTransparency();
			blend.m_z = static_cast<float>(particle.GetParticleType()->m_numberOfRows);

			ParticleTypeData instanceData = {};
			instanceData.modelMatrix = modelMatrix;
			instanceData.colourOffset = particle.GetParticleType()->m_colourOffset;
			instanceData.offsets = offsets;
			instanceData.blend = blend;
			instanceDatas[m_instances] = instanceData;
			m_instances++;

			if (m_instances >= m_maxInstances)
			{
				break;
			}
		}

		m_storageInstances.Push(instanceDatas.data(), sizeof(ParticleTypeData) * m_maxInstances);
	}

	bool ParticleType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene)
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
		m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

		m_model->CmdRender(commandBuffer, m_instances);
		return true;
	}

	void ParticleType::Decode(const Metadata &metadata)
	{
		metadata.GetResource("Texture", m_texture);
		metadata.GetChild("Number Of Rows", m_numberOfRows);
		metadata.GetChild("Colour Offset", m_colourOffset);
		metadata.GetChild("Life Length", m_lifeLength);
		metadata.GetChild("Stage Cycles", m_stageCycles);
		metadata.GetChild("Scale", m_scale);
	}

	void ParticleType::Encode(Metadata &metadata) const
	{
		metadata.SetResource("Texture", m_texture);
		metadata.SetChild("Number Of Rows", m_numberOfRows);
		metadata.SetChild("Colour Offset", m_colourOffset);
		metadata.SetChild("Life Length", m_lifeLength);
		metadata.SetChild("Stage Cycles", m_stageCycles);
		metadata.SetChild("Scale", m_scale);
	}
}
