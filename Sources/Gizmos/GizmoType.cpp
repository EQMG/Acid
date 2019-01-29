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
		std::shared_ptr<Resource> resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<GizmoType>(resource);
		}

		auto model = metadata.GetResource<Model>("Model");
		auto lineThickness = metadata.GetChild<float>("Line Thickness");
		auto diffuse = metadata.GetChild<Colour>("Diffuse");
		auto result = std::make_shared<GizmoType>(model, lineThickness, diffuse);
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	std::shared_ptr<GizmoType> GizmoType::Create(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &diffuse)
	{
		Metadata metadata = Metadata();
		metadata.SetChild<std::shared_ptr<Model>>("Model", model);
		metadata.SetChild<float>("Line Thickness", lineThickness);
		metadata.SetChild<Colour>("Diffuse", diffuse);
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

	void GizmoType::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::shared_ptr<Model>>("Model", m_model);
		metadata.SetChild<float>("Line Thickness", m_lineThickness);
		metadata.SetChild<Colour>("Diffuse", m_diffuse);
	}
}
