#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Renderer/Buffers/InstanceBuffer.hpp"
#include "Renderer/Descriptors/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/Images/Image2d.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
class Particle;

/**
 * @brief Resource that represents a particle type.
 */
class ACID_EXPORT ParticleType :
	public Resource
{
public:
	/**
	 * Creates a new particle type, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The particle type with the requested values.
	 */
	static std::shared_ptr<ParticleType> Create(const Metadata &metadata);

	/**
	 * Creates a new particle type, or finds one with the same values.
	 * @param texture The particles texture.
	 * @param numberOfRows The number of texture rows.
	 * @param colourOffset The particles texture colour offset.
	 * @param lifeLength The averaged life length for the particle.
	 * @param stageCycles The amount of times stages will be shown.
	 * @param scale The averaged scale for the particle.
	 * @return The particle type with the requested values.
	 */
	static std::shared_ptr<ParticleType> Create(const std::shared_ptr<Image2d> &texture, const uint32_t &numberOfRows = 1, const Colour &colourOffset = Colour::Black,
		const float &lifeLength = 10.0f, const float &stageCycles = 1.0f, const float &scale = 1.0f);

	/**
	 * Creates a new particle type.
	 * @param texture The particles texture.
	 * @param numberOfRows The number of texture rows.
	 * @param colourOffset The particles texture colour offset.
	 * @param lifeLength The averaged life length for the particle.
	 * @param stageCycles The amount of times stages will be shown.
	 * @param scale The averaged scale for the particle.
	 */
	explicit ParticleType(std::shared_ptr<Image2d> texture, const uint32_t &numberOfRows = 1, const Colour &colourOffset = Colour::Black, const float &lifeLength = 10.0f,
		const float &stageCycles = 1.0f, const float &scale = 1.0f);

	void Update(const std::vector<Particle> &particles);

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0);

	const std::shared_ptr<Image2d> &GetTexture() const { return m_texture; }

	void SetTexture(const std::shared_ptr<Image2d> &texture) { m_texture = texture; }

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

private:
	struct ParticleTypeData
	{
		Matrix4 m_modelMatrix;
		Colour m_colourOffset;
		Vector4f m_offsets;
		Vector3f m_blend;
	};

	std::shared_ptr<Image2d> m_texture;
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
