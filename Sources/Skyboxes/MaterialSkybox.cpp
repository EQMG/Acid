#include "MaterialSkybox.hpp"

#include "Models/VertexModel.hpp"
#include "Renderer/Pipelines/Compute.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	MaterialSkybox::MaterialSkybox(std::shared_ptr<Cubemap> cubemap, const Colour &skyColour) :
		IMaterial(),
		m_cubemap(cubemap),
		m_skyColour(skyColour),
		m_blend(1.0f),
		m_fogColour(Colour()),
		m_fogLimits(Vector2(-1000000.0f, -1000000.0f)),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Skyboxes/Skybox.vert", "Shaders/Skyboxes/Skybox.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, {})))
	{
	}

	MaterialSkybox::~MaterialSkybox()
	{
	}

	void MaterialSkybox::Start()
	{
		/*auto result = std::make_shared<Cubemap>(m_cubemap->GetWidth(), m_cubemap->GetHeight());

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_GRAPHICS_BIT);
		Compute compute = Compute(ComputeCreate("Shaders/Ibl.comp", result->GetWidth(), result->GetHeight(), 16, {}));

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("writeColour", result);
		descriptorSet.Push("samplerColour", m_cubemap);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer);
		compute.CmdRender(commandBuffer);
		commandBuffer.End();
		commandBuffer.Submit();

		m_cubemap = result;*/
	}

	void MaterialSkybox::Update()
	{
	}

	void MaterialSkybox::Load(LoadedValue *value)
	{
		TrySetCubemap(value->GetChild("Cubemap Texture")->GetString(), value->GetChild("Cubemap Extension")->GetString());
		m_skyColour = value->GetChild("Sky Colour")->GetString();
	}

	void MaterialSkybox::Write(LoadedValue *destination)
	{
		destination->GetChild("Cubemap Texture", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetFilename());
		destination->GetChild("Cubemap Extension", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetExtension());
		destination->GetChild("Sky Colour", true)->SetString(m_skyColour.GetHex());
	}

	void MaterialSkybox::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("skyColour", m_skyColour);
		uniformObject.Push("fogColour", m_fogColour);
		uniformObject.Push("fogLimits", GetGameObject()->GetTransform().GetScaling().m_y * m_fogLimits);
		uniformObject.Push("blendFactor", m_blend);
	}

	void MaterialSkybox::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerCubemap", m_cubemap);
	}
}
