#include "GizmoType.hpp"

#include "Resources/Resources.hpp"
#include "Helpers/String.hpp"
#include "Scenes/Scenes.hpp"
#include "Gizmo.hpp"

namespace acid
{
	const uint32_t GizmoType::MAX_TYPE_INSTANCES = 512;

	std::shared_ptr<GizmoType> GizmoType::Resource(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &diffuse)
	{
		auto resource = Resources::Get()->Find(ToFilename(model, lineThickness, diffuse));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<GizmoType>(resource);
		}

		auto result = std::make_shared<GizmoType>(model, lineThickness, diffuse);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<GizmoType> GizmoType::Resource(const std::string &data)
	{
		auto split = String::Split(data, "_");
		auto model = Model::Resource(split[1]);
		float lineThickness = String::From<float>(split[2]);
		return Resource(model, lineThickness);
	}

	GizmoType::GizmoType(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &diffuse) :
		m_filename(ToFilename(model, lineThickness, diffuse)),
		m_model(model),
		m_lineThickness(lineThickness),
		m_diffuse(diffuse),
		m_descriptorSet(DescriptorsHandler()),
		m_storageInstances(StorageHandler())
	{
	}

	void GizmoType::Update(const std::vector<std::unique_ptr<Gizmo>> &gizmos)
	{
		auto instanceDatas = std::vector<GizmoTypeData>();
		instanceDatas.resize(MAX_TYPE_INSTANCES);
		m_instances = 0;

		for (auto &gizmo : gizmos)
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

	bool GizmoType::CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene)
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
		m_descriptorSet.BindDescriptor(commandBuffer);

		m_model->CmdRender(commandBuffer, m_instances);
		return true;
	}

	void GizmoType::Decode(const Metadata &metadata)
	{
		m_model = Model::Resource(metadata.GetChild<std::string>("Model"));
		m_lineThickness = metadata.GetChild<float>("Line Thickness");
	}

	void GizmoType::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Model", m_model == nullptr ? "" : m_model->GetFilename());
		metadata.SetChild<float>("Line Thickness", m_lineThickness);
	}

	std::string GizmoType::ToFilename(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &diffuse)
	{
		std::stringstream result;
		result << "GizmoType_" << (model == nullptr ? "nullptr" : model->GetFilename()) << "_" << lineThickness << "_" << diffuse;
		return result.str();
	}
}
