#pragma once

#include "../Maths/Matrix4.hpp"
#include "../Maths/Vector3.hpp"
#include "../Models/Model.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "../Textures/Cubemap.hpp"

namespace Flounder
{
	class Skybox
	{
	private:
		UniformBuffer *m_uniformObject;

		Cubemap *m_cubemap;
		Model *m_model;

		float m_size;
		float m_blend;

		Vector3 *m_rotation;
		Matrix4 *m_modelMatrix;
	public:
		Skybox(Cubemap *cubemap, Model *model, const float &size);

		~Skybox();

		void Update();

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);

		Cubemap *GetCubemap() const { return m_cubemap; }

		void SetCubemap(Cubemap *cubemap) { m_cubemap = cubemap; }

		Model *GetModel() const { return m_model; }

		void SetModel(Model *model) { m_model = model; }

		float GetSize() const { return m_size; }

		void SetSize(const float size) { m_size = size; }

		float GetBlend() const { return m_blend; }

		void SetBlend(const float blend) { m_blend = blend; }

		Vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(Vector3 *rotation) { m_rotation = rotation; }

		Matrix4 *GetModelMatrix() const { return m_modelMatrix; }
	};
}
