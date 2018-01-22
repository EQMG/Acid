#pragma once

#include <vector>
#include "../Maths/Matrix4.hpp"
#include "../Maths/Transform.hpp"
#include "../Physics/Aabb.hpp"
#include "../Models/Model.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class Terrain
	{
	private:
		UniformBuffer *m_uniformObject;

		std::vector<Model *> m_modelLods;
		int m_currentLod;

		Transform *m_transform;

		Aabb *m_aabb;
	public:
		static const int SIDE_LENGTH;
		static const std::vector<float> SQUARE_SIZES;
		static const std::vector<float> TEXTURE_SCALES;

		Terrain(const Transform &transform);

		~Terrain();

		void Update();

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);

		static int CalculateVertexCount(const int &terrainLength, const float &squareSize);

	private:
		void CreateLod(const int &lod);

		static std::string ToFilename(const float &x, const float &z, const int &lod);

	public:
		std::vector<Model *> GetModelLods() const { return m_modelLods; }

		Model *GetModel(const int &lod) const { return m_modelLods[lod]; }

		Transform *GetTransform() const { return m_transform; }

		void SetTransform(const Transform &transform) const;

		Aabb *GetAabb() const { return m_aabb; }
	};
}
