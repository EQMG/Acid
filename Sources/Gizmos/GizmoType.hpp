#pragma once

#include <string>
#include "Maths/Colour.hpp"
#include "Maths/Matrix4.hpp"
#include "Models/Model.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Resources/IResource.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class Gizmo;

	struct GizmoTypeData
	{
		Matrix4 modelMatrix;
		Colour diffuse;
	};

	class ACID_EXPORT GizmoType :
		public IResource
	{
	private:
		std::string m_filename;
		std::shared_ptr<Model> m_model;
		float m_lineThickness;
		Colour m_diffuse;

		uint32_t m_instances;

		DescriptorsHandler m_descriptorSet;
		StorageHandler m_storageInstances;
	public:
		static const uint32_t MAX_TYPE_INSTANCES;

		/// <summary>
		/// Will find an existing gizmo type with the same filename, or create a new gizmo type.
		/// </summary>
		/// <param name="model"> The model that the gizmo will render. </param>
		/// <param name="lineThickness"> The thickness that the model will be rendered at. </param>
		/// <param name="diffuse"> The default diffuse colour for gizmos. </param>
		static std::shared_ptr<GizmoType> Resource(const std::shared_ptr<Model> &model = nullptr, const float &lineThickness = 1.0f, const Colour &diffuse = Colour::WHITE);

		/// <summary>
		/// Will find an existing gizmo type with the same filename, or create a new gizmo type.
		/// </summary>
		/// <param name="data"> The combined data for the gizmo type. </param>
		static std::shared_ptr<GizmoType> Resource(const std::string &data);

		/// <summary>
		/// Creates a new gizmo type.
		/// </summary>
		/// <param name="model"> The model that the gizmo will render. </param>
		/// <param name="lineThickness"> The thickness that the model will be rendered at. </param>
		/// <param name="diffuse"> The default diffuse colour for gizmos. </param>
		explicit GizmoType(const std::shared_ptr<Model> &model = nullptr, const float &lineThickness = 1.0f, const Colour &diffuse = Colour::WHITE);

		void Update(const std::vector<std::unique_ptr<Gizmo>> &gizmos);

		bool CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene);

		void Decode(const Metadata &metadata);

		void Encode(Metadata &metadata) const;

		std::string GetFilename() override { return m_filename; }

		std::shared_ptr<Model> GetModel() const { return m_model; }

		void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

		float GetLineThickness() const { return m_lineThickness; }

		void SetLineThickness(const float &lineThickness) { m_lineThickness = lineThickness; }

		Colour GetDiffuse() const { return m_diffuse; }

		void SetDiffuse(const Colour &diffuse) { m_diffuse = diffuse; }
	private:
		static std::string ToFilename(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &diffuse);
	};
}
