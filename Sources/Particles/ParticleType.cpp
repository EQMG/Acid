#include "ParticleType.hpp"

#include "Resources/Resources.hpp"
#include "Maths/Maths.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Scenes/Scenes.hpp"
#include "Particle.hpp"

namespace acid
{
static const uint32_t MAX_INSTANCES = 1024;
//static const uint32_t INSTANCE_STEPS = 128;
static const float FRUSTUM_BUFFER = 1.4f;

std::shared_ptr<ParticleType> ParticleType::Create(const Node &node)
{
	if (auto resource = Resources::Get()->Find(node))
	{
		return std::dynamic_pointer_cast<ParticleType>(resource);
	}

	auto result = std::make_shared<ParticleType>(nullptr);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	//result->Load();
	return result;
}

std::shared_ptr<ParticleType> ParticleType::Create(const std::shared_ptr<Image2d> &image, uint32_t numberOfRows, const Colour &colourOffset, float lifeLength,
	float stageCycles, float scale)
{
	ParticleType temp(image, numberOfRows, colourOffset, lifeLength, stageCycles, scale);
	Node node;
	node << temp;
	return Create(node);
}

ParticleType::ParticleType(std::shared_ptr<Image2d> image, uint32_t numberOfRows, const Colour &colourOffset, float lifeLength, float stageCycles,
	float scale) :
	m_image(std::move(image)),
	m_model(ModelRectangle::Create(-0.5f, 0.5f)),
	m_numberOfRows(numberOfRows),
	m_colourOffset(colourOffset),
	m_lifeLength(lifeLength),
	m_stageCycles(stageCycles),
	m_scale(scale),
	m_instanceBuffer(sizeof(Instance) * MAX_INSTANCES)
{
}

void ParticleType::Update(const std::vector<Particle> &particles)
{
	// Calculates a max instance count over the time of the type. TODO: Allow decreasing max using a timer and average count over the delay.
	//uint32_t instances = INSTANCE_STEPS * static_cast<uint32_t>(std::ceil(static_cast<float>(particles.size()) / static_cast<float>(INSTANCE_STEPS)));
	//m_maxInstances = std::max(m_maxInstances, instances);
	m_maxInstances = MAX_INSTANCES;
	m_instances = 0;

	if (particles.empty())
	{
		return;
	}

	Instance *instances;
	m_instanceBuffer.MapMemory(reinterpret_cast<void **>(&instances));

	for (const auto &particle : particles)
	{
		if (m_instances >= m_maxInstances)
		{
			break;
		}

		if (!Scenes::Get()->GetCamera()->GetViewFrustum().SphereInFrustum(particle.GetPosition(), FRUSTUM_BUFFER * particle.GetScale()))
		{
			continue;
		}

		auto viewMatrix = Scenes::Get()->GetCamera()->GetViewMatrix();
		auto instance = &instances[m_instances];
		instance->m_modelMatrix = Matrix4().Translate(particle.GetPosition());

		for (uint32_t row = 0; row < 3; row++)
		{
			for (uint32_t col = 0; col < 3; col++)
			{
				instance->m_modelMatrix[row][col] = viewMatrix[col][row];
			}
		}

		instance->m_modelMatrix = instance->m_modelMatrix.Rotate(particle.GetRotation(), Vector3f::Front);
		instance->m_modelMatrix = instance->m_modelMatrix.Scale(Vector3f(particle.GetScale()));
		// TODO: Multiply MVP by View and Projection (And run update every frame?)

		instance->m_colourOffset = particle.GetParticleType()->m_colourOffset;
		instance->m_offsets = {particle.m_imageOffset1, particle.m_imageOffset2};
		instance->m_blend = {particle.m_imageBlendFactor, particle.m_transparency, static_cast<float>(particle.m_particleType->m_numberOfRows)};
		m_instances++;
	}

	m_instanceBuffer.UnmapMemory();
}

bool ParticleType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene)
{
	if (m_instances == 0)
	{
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("UniformScene", uniformScene);
	m_descriptorSet.Push("samplerColour", m_image);

	if (!m_descriptorSet.Update(pipeline))
	{
		return false;
	}

	// Draws the instanced objects.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

	VkBuffer vertexBuffers[2] = {m_model->GetVertexBuffer()->GetBuffer(), m_instanceBuffer.GetBuffer()};
	VkDeviceSize offsets[2] = {0, 0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 2, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, m_model->GetIndexBuffer()->GetBuffer(), 0, m_model->GetIndexType());
	vkCmdDrawIndexed(commandBuffer, m_model->GetIndexCount(), m_instances, 0, 0, 0);
	return true;
}

const Node &operator>>(const Node &node, ParticleType &particleType)
{
	node["image"].Get(particleType.m_image);
	node["numberOfRows"].Get(particleType.m_numberOfRows);
	node["colourOffset"].Get(particleType.m_colourOffset);
	node["lifeLength"].Get(particleType.m_lifeLength);
	node["stageCycles"].Get(particleType.m_stageCycles);
	node["scale"].Get(particleType.m_scale);
	return node;
}

Node &operator<<(Node &node, const ParticleType &particleType)
{
	node["image"].Set(particleType.m_image);
	node["numberOfRows"].Set(particleType.m_numberOfRows);
	node["colourOffset"].Set(particleType.m_colourOffset);
	node["lifeLength"].Set(particleType.m_lifeLength);
	node["stageCycles"].Set(particleType.m_stageCycles);
	node["scale"].Set(particleType.m_scale);
	return node;
}
}
