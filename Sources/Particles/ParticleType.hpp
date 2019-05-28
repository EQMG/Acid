#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Graphics/Buffers/InstanceBuffer.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Graphics/Images/Image2d.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
class Particle;

/**
 * @brief Resource that represents a particle type.
 */
class ParticleType :
	public Resource
{
public:
	class Instance
	{
	public:
		static Shader::VertexInput GetVertexInput(const uint32_t &baseBinding = 0)
		{
			std::vector<VkVertexInputBindingDescription> bindingDescriptions = { 
				VkVertexInputBindingDescription{ baseBinding, sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE }
			};
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
				VkVertexInputAttributeDescription{ 0, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_modelMatrix) + offsetof(Matrix4, m_rows[0]) },
				VkVertexInputAttributeDescription{ 1, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_modelMatrix) + offsetof(Matrix4, m_rows[1]) },
				VkVertexInputAttributeDescription{ 2, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_modelMatrix) + offsetof(Matrix4, m_rows[2]) },
				VkVertexInputAttributeDescription{ 3, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_modelMatrix) + offsetof(Matrix4, m_rows[3]) },
				VkVertexInputAttributeDescription{ 4, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_colourOffset) },
				VkVertexInputAttributeDescription{ 5, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_offsets) },
				VkVertexInputAttributeDescription{ 6, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, m_blend) }
			};
			return Shader::VertexInput(bindingDescriptions, attributeDescriptions);
		}

		Matrix4 m_modelMatrix;
		Colour m_colourOffset;
		Vector4f m_offsets;
		Vector3f m_blend;
	};

	/**
	 * Creates a new particle type, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The particle type with the requested values.
	 */
	static std::shared_ptr<ParticleType> Create(const Metadata &metadata);

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
	static std::shared_ptr<ParticleType> Create(const std::shared_ptr<Image2d> &image, const uint32_t &numberOfRows = 1, const Colour &colourOffset = Colour::Black,
		const float &lifeLength = 10.0f, const float &stageCycles = 1.0f, const float &scale = 1.0f);

	/**
	 * Creates a new particle type.
	 * @param image The particles image.
	 * @param numberOfRows The number of Image rows.
	 * @param colourOffset The particles Image colour offset.
	 * @param lifeLength The averaged life length for the particle.
	 * @param stageCycles The amount of times stages will be shown.
	 * @param scale The averaged scale for the particle.
	 */
	explicit ParticleType(std::shared_ptr<Image2d> image, const uint32_t &numberOfRows = 1, const Colour &colourOffset = Colour::Black, const float &lifeLength = 10.0f,
		const float &stageCycles = 1.0f, const float &scale = 1.0f);

	void Update(const std::vector<Particle> &particles);

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

	const std::shared_ptr<Image2d> &GetImage() const { return m_image; }

	void SetImage(const std::shared_ptr<Image2d> &image) { m_image = image; }

	const uint32_t &GetNumberOfRows() const { return m_numberOfRows; }

	void SetNumberOfRows(const uint32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

	const Colour &GetColourOffset() const { return m_colourOffset; }

	void SetColourOffset(const Colour &colourOffset) { m_colourOffset = colourOffset; }

	const float &GetLifeLength() const { return m_lifeLength; }

	void SetLifeLength(const float &lifeLength) { m_lifeLength = lifeLength; }

	const float &GetStageCycles() const { return m_stageCycles; }

	void SetStageCycles(const float &stageCycles) { m_stageCycles = stageCycles; }

	const float &GetScale() const { return m_scale; }

	void SetScale(const float &scale) { m_scale = scale; }

	friend const Metadata &operator>>(const Metadata &metadata, ParticleType &particleType);

	friend Metadata &operator<<(Metadata &metadata, const ParticleType &particleType);

private:
	std::shared_ptr<Image2d> m_image;
	std::shared_ptr<Model> m_model;
	uint32_t m_numberOfRows;
	Colour m_colourOffset;
	float m_lifeLength;
	float m_stageCycles;
	float m_scale;

	uint32_t m_maxInstances;
	uint32_t m_instances;

	DescriptorsHandler m_descriptorSet;
	InstanceBuffer m_instanceBuffer;
};
}
