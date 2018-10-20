#pragma once

#include <optional>
#include <Objects/IComponent.hpp>
#include <Meshes/Mesh.hpp>
#include "VoxelBlock.hpp"

using namespace acid;

namespace test
{
	enum ChunkMesher
	{
		MESHER_GREEDY = 0,
		MESHER_CULLED = 1
	};

	class VoxelChunk :
		public IComponent
	{
	private:
		std::vector<std::vector<std::vector<VoxelBlock>>> m_blocks;
		ChunkMesher m_mesher;
		bool m_generate;
		bool m_rebuild;
	public:
		static const uint32_t CHUNK_WIDTH;
		static const uint32_t CHUNK_HEIGHT;
		static const float VOXEL_SIZE;
		static const Vector3 CHUNK_SIZE;

		explicit VoxelChunk(const ChunkMesher &mesher = MESHER_GREEDY, const bool &generate = false);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		std::optional<VoxelBlock> GetBlock(const int32_t &x, const int32_t &y, const int32_t &z);

		void SetBlockType(const int32_t &x, const int32_t &y, const int32_t &z, const std::string &type);

		bool IsBlockFilled(const int32_t &x, const int32_t &y, const int32_t &z);

		bool IsFaceVisible(const int32_t &x, const int32_t &y, const int32_t &z, const BlockFace &faceType);

		void Rebuild() { m_rebuild = true; }
	private:
		void Generate();

		void GenerateMesh();

		void CreateGreedyMesh(std::vector<VertexModel> &vertices, std::vector<uint32_t> &indices);

		void CreateCulledMesh(std::vector<VertexModel> &vertices, std::vector<uint32_t> &indices);

		std::string GetVoxelFace(const int32_t &x, const int32_t &y, const int32_t &z, const BlockFace &faceType);

		void GenerateQuad(std::vector<VertexModel> &vertices, std::vector<uint32_t> &indices,
		                  const Vector3 &bottomLeft, const Vector3 &topLeft, const Vector3 &topRight, const Vector3 &bottomRight,
		                  const int32_t &width, const int32_t &height,
		                  const std::string &blockType, const bool &backFace);
	};
}
