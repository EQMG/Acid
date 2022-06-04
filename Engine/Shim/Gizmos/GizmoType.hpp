#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Matrix4.hpp"
#include "Resources/Resource.hpp"
#include "Files/Node.hpp"
#include "Models/Model.hpp"
#include "Graphics/Buffers/InstanceBuffer.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "ShimExport.hpp"

namespace acid {
class Gizmo;

/**
 * @brief Resource that represents a gizmo type.
 */
class ACID_SHIM_EXPORT GizmoType : public Resource {
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
				{4, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, colour)}
			};
			return {bindingDescriptions, attributeDescriptions};
		}

		Matrix4 modelMatrix;
		Colour colour;
	};

	/**
	 * Creates a new gizmo type, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The gizmo type with the requested values.
	 */
	static std::shared_ptr<GizmoType> Create(const Node &node);

	/**
	 * Creates a new gizmo type, or finds one with the same values.
	 * @param model The model that the gizmo will render.
	 * @param lineThickness The thickness that the model will be rendered at.
	 * @param colour The default colour for gizmos.
	 * @return The gizmo type with the requested values.
	 */
	static std::shared_ptr<GizmoType> Create(const std::shared_ptr<Model> &model = nullptr, float lineThickness = 1.0f, const Colour &colour = Colour::White);

	/**
	 * Creates a new gizmo type.
	 * @param model The model that the gizmo will render.
	 * @param lineThickness The thickness that the model will be rendered at.
	 * @param colour The default colour for gizmos.
	 */
	explicit GizmoType(std::shared_ptr<Model> model, float lineThickness = 1.0f, const Colour &colour = Colour::White);

	void Update(const std::vector<std::unique_ptr<Gizmo>> &gizmos);

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

	std::type_index GetTypeIndex() const override { return typeid(GizmoType); }

	const std::shared_ptr<Model> &GetModel() const { return model; }
	void SetModel(const std::shared_ptr<Model> &model) { this->model = model; }

	float GetLineThickness() const { return lineThickness; }
	void SetLineThickness(float lineThickness) { this->lineThickness = lineThickness; }

	const Colour &GetColour() const { return colour; }
	void SetColour(const Colour &colour) { this->colour = colour; }

	friend const Node &operator>>(const Node &node, GizmoType &gizmoType);
	friend Node &operator<<(Node &node, const GizmoType &gizmoType);

private:
	std::shared_ptr<Model> model;
	float lineThickness;
	Colour colour;

	uint32_t maxInstances = 0;
	uint32_t instances = 0;

	DescriptorsHandler descriptorSet;
	InstanceBuffer instanceBuffer;
};
}
