#pragma once

#include "../Objects/Component.hpp"
#include "../Meshes/Mesh.hpp"
#include "Block.hpp"

namespace Flounder
{
	typedef std::vector<std::vector<std::vector<Block*>>> BlockVector;

	class F_EXPORT Chunk :
		public Component
	{
	private:
		BlockVector *m_blocks;
		bool m_generate;
		bool m_rebuild;
	public:
		static const unsigned int SIDE_LENGTH;
		static const unsigned int MAX_HEIGHT;

		Chunk(const bool &generate = true);

		~Chunk();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Chunk"; };

		Block *GetBlock(const unsigned int &x, const unsigned int &y, const unsigned int &z);

		bool IsBlockFilled(const unsigned int &x, const unsigned int &y, const unsigned int &z);

		void Rebuild() { m_rebuild = true; }

	private:
		void Generate();

		void GenerateMesh();

		void AppendFace(const std::vector<Vertex> &dataVertices, const std::vector<uint32_t> &dataIndices, const Block &block, std::vector<Vertex> *vertices, std::vector<uint32_t> *indices);
	};
}
