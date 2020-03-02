#include "ParticleType.hpp"

#include "Resources/Resources.hpp"
#include "Maths/Maths.hpp"
#include "Models/Shapes/RectangleModel.hpp"
#include "Scenes/Scenes.hpp"
#include "Particle.hpp"

namespace acid {
static const uint32_t MAX_INSTANCES = 1024;
//static const uint32_t INSTANCE_STEPS = 128;
static const float FRUSTUM_BUFFER = 1.4f;

std::shared_ptr<ParticleType> ParticleType::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<ParticleType>(node))
		return resource;

	auto result = std::make_shared<ParticleType>(nullptr);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	//result->Load();
	return result;
}

std::shared_ptr<ParticleType> ParticleType::Create(const std::shared_ptr<Image2d> &image, uint32_t numberOfRows, const Colour &colourOffset, float lifeLength,
	float stageCycles, float scale) {
	ParticleType temp(image, numberOfRows, colourOffset, lifeLength, stageCycles, scale);
	Node node;
	node << temp;
	return Create(node);
}

ParticleType::ParticleType(std::shared_ptr<Image2d> image, uint32_t numberOfRows, const Colour &colourOffset, float lifeLength, float stageCycles,
	float scale) :
	image(std::move(image)),
	model(RectangleModel::Create(-0.5f, 0.5f)),
	numberOfRows(numberOfRows),
	colourOffset(colourOffset),
	lifeLength(lifeLength),
	stageCycles(stageCycles),
	scale(scale),
	instanceBuffer(sizeof(Instance) * MAX_INSTANCES) {
}

void ParticleType::Update(const std::vector<Particle> &particles) {
	// Calculates a max instance count over the time of the type. TODO: Allow decreasing max using a timer and average count over the delay.
	//uint32_t instances = INSTANCE_STEPS * static_cast<uint32_t>(std::ceil(static_cast<float>(particles.size()) / static_cast<float>(INSTANCE_STEPS)));
	//maxInstances = std::max(maxInstances, instances);
	maxInstances = MAX_INSTANCES;
	this->instances = 0;

	if (particles.empty()) {
		return;
	}

	Instance *instances;
	instanceBuffer.MapMemory(reinterpret_cast<void **>(&instances));

	for (const auto &particle : particles) {
		if (this->instances >= maxInstances) {
			break;
		}

		if (!Scenes::Get()->GetCamera()->GetViewFrustum().SphereInFrustum(particle.GetPosition(), FRUSTUM_BUFFER * particle.GetScale())) {
			continue;
		}

		auto viewMatrix = Scenes::Get()->GetCamera()->GetViewMatrix();
		auto instance = &instances[this->instances];
		instance->modelMatrix = Matrix4().Translate(particle.GetPosition());

		for (uint32_t row = 0; row < 3; row++) {
			for (uint32_t col = 0; col < 3; col++) {
				instance->modelMatrix[row][col] = viewMatrix[col][row];
			}
		}

		instance->modelMatrix = instance->modelMatrix.Rotate(particle.GetRotation(), Vector3f::Front);
		instance->modelMatrix = instance->modelMatrix.Scale(Vector3f(particle.GetScale()));
		// TODO: Multiply MVP by View and Projection (And run update every frame?)

		instance->colourOffset = particle.GetParticleType()->colourOffset;
		instance->offsets = {particle.imageOffset1, particle.imageOffset2};
		instance->blend = {particle.imageBlendFactor, particle.transparency, static_cast<float>(particle.particleType->numberOfRows)};
		instances++;
	}

	instanceBuffer.UnmapMemory();
}

bool ParticleType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene) {
	if (instances == 0)
		return false;

	// Updates descriptors.
	descriptorSet.Push("UniformScene", uniformScene);
	descriptorSet.Push("samplerColour", image);

	if (!descriptorSet.Update(pipeline))
		return false;

	// Draws the instanced objects.
	descriptorSet.BindDescriptor(commandBuffer, pipeline);

	VkBuffer vertexBuffers[2] = {model->GetVertexBuffer()->GetBuffer(), instanceBuffer.GetBuffer()};
	VkDeviceSize offsets[2] = {0, 0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 2, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, model->GetIndexBuffer()->GetBuffer(), 0, model->GetIndexType());
	vkCmdDrawIndexed(commandBuffer, model->GetIndexCount(), instances, 0, 0, 0);
	return true;
}

const Node &operator>>(const Node &node, ParticleType &particleType) {
	node["image"].Get(particleType.image);
	node["numberOfRows"].Get(particleType.numberOfRows);
	node["colourOffset"].Get(particleType.colourOffset);
	node["lifeLength"].Get(particleType.lifeLength);
	node["stageCycles"].Get(particleType.stageCycles);
	node["scale"].Get(particleType.scale);
	return node;
}

Node &operator<<(Node &node, const ParticleType &particleType) {
	node["image"].Set(particleType.image);
	node["numberOfRows"].Set(particleType.numberOfRows);
	node["colourOffset"].Set(particleType.colourOffset);
	node["lifeLength"].Set(particleType.lifeLength);
	node["stageCycles"].Set(particleType.stageCycles);
	node["scale"].Set(particleType.scale);
	return node;
}
}
