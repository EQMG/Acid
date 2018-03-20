#pragma once

#include "../Objects/Component.hpp"
#include "../Meshes/Mesh.hpp"
#include "Block.hpp"

namespace Flounder
{
	enum ChunkMesh
	{
		MeshGreedy = 0,
		MeshSimple = 1
	};

	class F_EXPORT Chunk :
		public Component
	{
	private:
		std::vector<std::vector<std::vector<Block*>>> *m_blocks;
		ChunkMesh m_chunkMesh;
		bool m_generate;
		bool m_rebuild;
	public:
		static const int CHUNK_WIDTH;
		static const int CHUNK_HEIGHT;
		static const float VOXEL_SIZE;
		static const Vector3 *CHUNK_SIZE;

		Chunk(const ChunkMesh &chunkMesh = MeshGreedy, const bool &generate = false);

		~Chunk();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Chunk"; };

		Block *GetBlock(const int &x, const int &y, const int &z);

		bool IsBlockFilled(const int &x, const int &y, const int &z);

		bool IsFaceVisible(const int &x, const int &y, const int &z, const BlockFace &faceType);

		void Rebuild() { m_rebuild = true; }

	private:
		void Generate();

		void GenerateMesh();

		void CreateSimpleMesh(std::vector<VertexModel> *vertices, std::vector<uint32_t> *indices);

		void CreateGreedyMesh(std::vector<VertexModel> *vertices, std::vector<uint32_t> *indices);

		std::string GetVoxelFace(const int &x, const int &y, const int &z, const BlockFace &faceType);

		void GenerateQuad(std::vector<VertexModel> *vertices, std::vector<uint32_t> *indices,
						  const Vector3 &bottomLeft, const Vector3 &topLeft, const Vector3 &topRight, const Vector3 &bottomRight,
						  const int &width, const int &height,
						  const std::string &blockType, const bool &backFace);
	};
}
