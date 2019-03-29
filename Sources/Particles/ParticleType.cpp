#include "ParticleType.hpp"

#include "Resources/Resources.hpp"
#include "Maths/Maths.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Scenes/Scenes.hpp"
#include "Particle.hpp"

namespace acid
{
	static const uint32_t MAX_INSTANCES = 1024;
//	static const uint32_t INSTANCE_STEPS = 128;
	static const float FRUSTUM_BUFFER = 1.4f;

	std::shared_ptr<ParticleType> ParticleType::Create(const Metadata &metadata)
	{
		auto resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ParticleType>(resource);
		}

		auto result = std::make_shared<ParticleType>(nullptr);
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<ParticleType> ParticleType::Create(const std::shared_ptr<Image2d> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, 
		const float &lifeLength, const float &stageCycles, const float &scale)
	{
		auto temp = ParticleType(texture, numberOfRows, colourOffset, lifeLength, stageCycles, scale);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	ParticleType::ParticleType(std::shared_ptr<Image2d> texture, const uint32_t &numberOfRows, const Colour &colourOffset,
		const float &lifeLength, const float &stageCycles, const float &scale) :
		m_texture(std::move(texture)),
		m_model(ModelRectangle::Create(-0.5f, 0.5f)),
		m_numberOfRows(numberOfRows),
		m_colourOffset(colourOffset),
		m_lifeLength(lifeLength),
		m_stageCycles(stageCycles),
		m_scale(scale),
		m_maxInstances(0),
		m_instances(0),
		m_instanceBuffer(sizeof(ParticleTypeData) * MAX_INSTANCES)
	{
	}

	void ParticleType::Update(const std::vector<Particle> &particles)
	{
		// Calculates a max instance count over the time of the type. TODO: Allow decreasing max using a timer and average count over the delay.
	//	uint32_t instances = INSTANCE_STEPS * static_cast<uint32_t>(std::ceil(static_cast<float>(particles.size()) / static_cast<float>(INSTANCE_STEPS)));
	//	m_maxInstances = std::max(m_maxInstances, instances);
		m_maxInstances = MAX_INSTANCES;
		m_instances = 0;

		if (particles.empty())
		{
			return;
		}

		ParticleTypeData *particleInstances;
		m_instanceBuffer.Map(reinterpret_cast<void **>(&particleInstances));

		for (const auto &particle : particles)
		{
			if (m_instances >= m_maxInstances)
			{
				break;
			}

			if (!Scenes::Get()->GetCamera()->GetViewFrustum().SphereInFrustum(particle.GetPosition(), FRUSTUM_BUFFER *particle.GetScale()))
			{
				continue;
			}

			auto viewMatrix = Scenes::Get()->GetCamera()->GetViewMatrix();
			ParticleTypeData *instance = &particleInstances[m_instances];
			instance->modelMatrix = Matrix4::Identity.Translate(particle.GetPosition());

			for (int32_t row = 0; row < 3; row++)
			{
				for (int32_t col = 0; col < 3; col++)
				{
					instance->modelMatrix[row][col] = viewMatrix[col][row];
				}
			}

			instance->modelMatrix = instance->modelMatrix.Rotate(particle.GetRotation() * Maths::DegToRad, Vector3::Front);
			instance->modelMatrix = instance->modelMatrix.Scale(particle.GetScale() * Vector3::One);
			// TODO: Multiply MVP by View and Projection (And run update every frame?)

			instance->colourOffset = particle.GetParticleType()->m_colourOffset;
			instance->offsets = Vector4(particle.GetTextureOffset1(), particle.GetTextureOffset2());
			instance->blend = Vector3(particle.GetTextureBlendFactor(), particle.GetTransparency(),
				static_cast<float>(particle.GetParticleType()->m_numberOfRows));
			m_instances++;
		}

		m_instanceBuffer.Unmap();
	}

	bool ParticleType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene)
	{
		if (m_instances == 0)
		{
			return false;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("samplerColour", m_texture);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		// Draws the instanced objects.
		m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

		VkBuffer vertexBuffers[] = {m_model->GetVertexBuffer()->GetBuffer(), m_instanceBuffer.GetBuffer()};
		VkDeviceSize offsets[] = {0, 0};
		vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 2, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer.GetCommandBuffer(), m_model->GetIndexBuffer()->GetBuffer(), 0, m_model->GetIndexType());
		vkCmdDrawIndexed(commandBuffer.GetCommandBuffer(), m_model->GetIndexCount(), m_instances, 0, 0, 0);
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

	Shader::VertexInput ParticleType::GetVertexInput(const uint32_t &binding)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = binding;
		bindingDescriptions[0].stride = sizeof(ParticleTypeData);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(7);

		// Model matrix row 0 attribute.
		attributeDescriptions[0].binding = binding;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(ParticleTypeData, modelMatrix) + offsetof(Matrix4, m_rows[0]);

		// Model matrix row 1 attribute.
		attributeDescriptions[1].binding = binding;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(ParticleTypeData, modelMatrix) + offsetof(Matrix4, m_rows[1]);

		// Model matrix row 2 attribute.
		attributeDescriptions[2].binding = binding;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(ParticleTypeData, modelMatrix) + offsetof(Matrix4, m_rows[2]);

		// Model matrix row 3 attribute.
		attributeDescriptions[3].binding = binding;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(ParticleTypeData, modelMatrix) + offsetof(Matrix4, m_rows[3]);

		// Colour offset attribute.
		attributeDescriptions[4].binding = binding;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[4].offset = offsetof(ParticleTypeData, colourOffset);

		// Offsets attribute.
		attributeDescriptions[5].binding = binding;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[5].offset = offsetof(ParticleTypeData, offsets);

		// Blend attribute.
		attributeDescriptions[6].binding = binding;
		attributeDescriptions[6].location = 6;
		attributeDescriptions[6].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[6].offset = offsetof(ParticleTypeData, blend);

		return Shader::VertexInput(binding, bindingDescriptions, attributeDescriptions);
	}
}
