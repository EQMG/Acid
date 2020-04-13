#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Matrix4.hpp"
#include "Models/Model.hpp"
#include "Graphics/Buffers/InstanceBuffer.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Graphics/Images/Image2d.hpp"
#include "Resources/Resource.hpp"

namespace acid {
class Particle;

/**
 * @brief Resource that represents a particle type.
 */
class ACID_EXPORT ParticleType : public Resource {
public:
	class Instance {
	public:
		static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0) {
			std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
				{baseBinding, sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE}
			};
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
				{0, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, modelMatrix) + offsetof(Matrix4, rows[0])},
				{1, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, modelMatrix) + offsetof(Matrix4, rows[1])},
				{2, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, modelMatrix) + offsetof(Matrix4, rows[2])},
				{3, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, modelMatrix) + offsetof(Matrix4, rows[3])},
				{4, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, colourOffset)},
				{5, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, offsets)},
				{6, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, blend)}
			};
			return {bindingDescriptions, attributeDescriptions};
		}

		Matrix4 modelMatrix;
		Colour colourOffset;
		Vector4f offsets;
		Vector3f blend;
	};

	/**
	 * Creates a new particle type, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The particle type with the requested values.
	 */
	static std::shared_ptr<ParticleType> Create(const Node &node);

	/**
	 * Creates a new particle type, or finds one with the same values.
	 * @param image The particles image.
	 * @param numberOfRows The number of Image rows.
	 * @param colourOffset The particles Image colour offset.
	 * @param lifeLength The averaged life length for the particle.
	 * @param stageCycles The amount of times stages will be shown.
	 * @param scale The averaged scale for the particle.
	 * @return The particle type with the requested values.
	 */
	static std::shared_ptr<ParticleType> Create(const std::shared_ptr<Image2d> &image, uint32_t numberOfRows = 1, const Colour &colourOffset = Colour::Black,
		float lifeLength = 10.0f, float stageCycles = 1.0f, float scale = 1.0f);

	/**
	 * Creates a new particle type.
	 * @param image The particles image.
	 * @param numberOfRows The number of Image rows.
	 * @param colourOffset The particles Image colour offset.
	 * @param lifeLength The averaged life length for the particle.
	 * @param stageCycles The amount of times stages will be shown.
	 * @param scale The averaged scale for the particle.
	 */
	explicit ParticleType(std::shared_ptr<Image2d> image, uint32_t numberOfRows = 1, const Colour &colourOffset = Colour::Black, float lifeLength = 10.0f,
		float stageCycles = 1.0f, float scale = 1.0f);

	void Update(const std::vector<Particle> &particles);

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

	std::type_index GetTypeIndex() const override { return typeid(ParticleType); }

	const std::shared_ptr<Image2d> &GetImage() const { return image; }
	void SetImage(const std::shared_ptr<Image2d> &image) { this->image = image; }

	uint32_t GetNumberOfRows() const { return numberOfRows; }
	void SetNumberOfRows(uint32_t numberOfRows) { this->numberOfRows = numberOfRows; }

	const Colour &GetColourOffset() const { return colourOffset; }
	void SetColourOffset(const Colour &colourOffset) { this->colourOffset = colourOffset; }

	float GetLifeLength() const { return lifeLength; }
	void SetLifeLength(float lifeLength) { this->lifeLength = lifeLength; }

	float GetStageCycles() const { return stageCycles; }
	void SetStageCycles(float stageCycles) { this->stageCycles = stageCycles; }

	float GetScale() const { return scale; }
	void SetScale(float scale) { this->scale = scale; }

	friend const Node &operator>>(const Node &node, ParticleType &particleType);
	friend Node &operator<<(Node &node, const ParticleType &particleType);

private:
	std::shared_ptr<Image2d> image;
	std::shared_ptr<Model> model;
	uint32_t numberOfRows;
	Colour colourOffset;
	float lifeLength;
	float stageCycles;
	float scale;

	uint32_t maxInstances = 0;
	uint32_t instances = 0;

	DescriptorsHandler descriptorSet;
	InstanceBuffer instanceBuffer;
};
}
