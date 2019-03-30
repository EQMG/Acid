#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Matrix4.hpp"
#include "Models/Model.hpp"
#include "Renderer/Buffers/InstanceBuffer.hpp"
#include "Renderer/Descriptors/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Resources/Resource.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
class Gizmo;

class ACID_EXPORT GizmoType :
	public Resource
{
public:
	/// <summary>
	/// Will find an existing gizmo type with the same values, or create a new gizmo type.
	/// </summary>
	/// <param name="metadata"> The metadata to decode values from. </param>
	static std::shared_ptr<GizmoType> Create(const Metadata &metadata);

	/// <summary>
	/// Will find an existing gizmo type with the same values, or create a new gizmo type.
	/// </summary>
	/// <param name="model"> The model that the gizmo will render. </param>
	/// <param name="lineThickness"> The thickness that the model will be rendered at. </param>
	/// <param name="colour"> The default colour for gizmos. </param>
	static std::shared_ptr<GizmoType> Create(const std::shared_ptr<Model> &model = nullptr, const float &lineThickness = 1.0f, const Colour &colour = Colour::White);

	/// <summary>
	/// Creates a new gizmo type.
	/// </summary>
	/// <param name="model"> The model that the gizmo will render. </param>
	/// <param name="lineThickness"> The thickness that the model will be rendered at. </param>
	/// <param name="colour"> The default colour for gizmos. </param>
	explicit GizmoType(std::shared_ptr<Model> model, const float &lineThickness = 1.0f, const Colour &colour = Colour::White);

	void Update(const std::vector<std::unique_ptr<Gizmo>> &gizmos);

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0);

	const std::shared_ptr<Model> &GetModel() const { return m_model; }

	void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

	const float &GetLineThickness() const { return m_lineThickness; }

	void SetLineThickness(const float &lineThickness) { m_lineThickness = lineThickness; }

	const Colour &GetColour() const { return m_colour; }

	void SetColour(const Colour &colour) { m_colour = colour; }

private:
	struct GizmoTypeData
	{
		Matrix4 modelMatrix;
		Colour colour;
	};

	std::shared_ptr<Model> m_model;
	float m_lineThickness;
	Colour m_colour;

	uint32_t m_maxInstances;
	uint32_t m_instances;

	DescriptorsHandler m_descriptorSet;
	InstanceBuffer m_instanceBuffer;
};
}
