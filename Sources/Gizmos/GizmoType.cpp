#include "GizmoType.hpp"

#include "Resources/Resources.hpp"
#include "Helpers/String.hpp"
#include "Scenes/Scenes.hpp"
#include "Gizmo.hpp"

namespace acid
{
	static const uint32_t MAX_TYPE_INSTANCES = 512;

	std::shared_ptr<GizmoType> GizmoType::Create(const Metadata &metadata)
	{
		auto result = std::make_shared<GizmoType>(nullptr);
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<GizmoType> GizmoType::Create(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &diffuse)
	{
		auto temp = GizmoType(model, lineThickness, diffuse);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	GizmoType::GizmoType(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &diffuse) :
		m_model(model),
		m_lineThickness(lineThickness),
		m_diffuse(diffuse),
		m_instances(0),
		m_descriptorSet(DescriptorsHandler()),
		m_storageInstances(StorageHandler())
	{
	}

	void GizmoType::Update(const std::vector<std::unique_ptr<Gizmo>> &gizmos)
	{
		std::vector<GizmoTypeData> instanceDatas(MAX_TYPE_INSTANCES);
		m_instances = 0;

		for (const auto &gizmo : gizmos)
		{
			GizmoTypeData instanceData = {};
			instanceData.modelMatrix = gizmo->GetTransform().GetWorldMatrix();
			instanceData.diffuse = gizmo->GetDiffuse();			
			instanceDatas[m_instances] = instanceData;

			m_instances++;

			if (m_instances >= instanceDatas.size())
			{
				break;
			}
		}

		m_storageInstances.Push(instanceDatas.data(), sizeof(GizmoTypeData) * MAX_TYPE_INSTANCES);
	}

	bool GizmoType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene)
	{
		if (m_instances == 0)
		{
			return false;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("Instances", m_storageInstances);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		vkCmdSetLineWidth(commandBuffer.GetCommandBuffer(), m_lineThickness);

		// Draws the instanced objects.
		m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

		m_model->CmdRender(commandBuffer, m_instances);
		return true;
	}

	void GizmoType::Decode(const Metadata &metadata)
	{
		metadata.GetResource("Model", m_model);
		metadata.GetChild("Line Thickness", m_lineThickness);
		metadata.GetChild("Diffuse", m_diffuse);
	}

	void GizmoType::Encode(Metadata &metadata) const
	{
		metadata.SetResource("Model", m_model);
		metadata.SetChild("Line Thickness", m_lineThickness);
		metadata.SetChild("Diffuse", m_diffuse);
	}
}
