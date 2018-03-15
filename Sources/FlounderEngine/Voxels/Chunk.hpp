#pragma once

#include "../Objects/Component.hpp"
#include "../Meshes/Mesh.hpp"
#include "Block.hpp"
#include "BlockFace.hpp"

namespace Flounder
{
	typedef std::vector<std::vector<std::vector<Block*>>> BlockVector;

	class F_EXPORT Chunk :
		public Component
	{
	private:
		Vector3 *m_position;
		BlockVector *m_blocks;
		bool m_generate;
		bool m_rebuild;
	public:
		static const unsigned int CHUNK_WIDTH;
		static const unsigned int CHUNK_HEIGHT;
		static const Vector3 *CHUNK_SIZE;

		Chunk(const bool &generate = false);

		~Chunk();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Chunk"; };

		Block *GetBlock(const unsigned int &x, const unsigned int &y, const unsigned int &z);

		bool IsBlockFilled(const unsigned int &x, const unsigned int &y, const unsigned int &z);

		bool IsFaceVisible(const unsigned int &x, const unsigned int &y, const unsigned int &z, const BlockFaceType &faceType);

		void Rebuild() { m_rebuild = true; }

	private:
		void Generate();

		void GenerateMesh();

		void CreateSimpleMesh(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices);

		void CreateGreedyMesh(std::vector<Vertex> *vertices, std::vector<uint32_t> *indices);

		BlockFaceType GetVoxelFace(const int &x, const int &y, const int &z, const BlockFaceType &faceType);
	};
}
