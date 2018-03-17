#pragma once

#include "../Objects/Component.hpp"
#include "../Meshes/Mesh.hpp"
#include "Block.hpp"
#include "BlockFace.hpp"

namespace Flounder
{
	typedef std::vector<std::vector<std::vector<Block*>>> BlockVector;

	enum ChunkMesh
	{
		MeshGreedy = 0,
		MeshSimple = 1
	};

	class F_EXPORT Chunk :
		public Component
	{
	private:
		ChunkMesh m_chunkMesh;
		BlockVector *m_blocks;
		std::vector<Chunk *> *m_surrounding;
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

		Block *GetBlock(const int &x, const int &y, const int &z, const bool &onlyThis = false);

		bool IsBlockFilled(const int &x, const int &y, const int &z, const bool &onlyThis = false);

		bool IsFaceVisible(const int &x, const int &y, const int &z, const FaceSide &faceType, const bool &onlyThis = false);

		void Rebuild() { m_rebuild = true; }

	private:
		void Generate();

		void GenerateMesh();

		void CreateSimpleMesh(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices);

		void CreateGreedyMesh(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices);

		short GetVoxelFace(const int &x, const int &y, const int &z, const FaceSide &faceType);

		void GenerateQuad(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices,
						  const Vector3 &bottomLeft, const Vector3 &topLeft, const Vector3 &topRight, const Vector3 &bottomRight,
						  const int &width, const int &height,
						  const short &mask, const FaceSide &faceSide, const bool &backFace);
	};
}
